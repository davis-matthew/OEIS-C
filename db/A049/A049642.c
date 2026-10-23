/*
 * A049642  Numbers k such that the number of divisors of k does not divide
 *          the sum of divisors of k.
 * https://oeis.org/A049642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive integer k such that the number of
 *            divisors τ(k) does not divide the sum of divisors σ(k)
 * Range:     u8 n <= 17, u16 n <= 794, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*** static data ********************************************/

static uint64_t *primes = NULL;
static size_t   primes_len   = 0;
static size_t   primes_cap   = 0;
static uint64_t last_prime_checked = 1;

static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap   = 0;
static uint64_t current_k = 1;  /* last checked integer */

/*** helper functions ****************************************/

static void grow_primes_storage(size_t new_cap)
{
    if (new_cap <= primes_cap)
        return;
    size_t cap = primes_cap ? primes_cap * 2 : 16;
    while (cap < new_cap)
        cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
    if (!tmp && cap > 0)
        exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = cap;
}

static void grow_seq_cache(size_t new_cap)
{
    if (new_cap <= seq_cache_cap)
        return;
    size_t cap = seq_cache_cap ? seq_cache_cap * 2 : 4;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, cap * sizeof(seq_t));
    if (!tmp && cap > 0)
        exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cache_cap = cap;
}

static uint64_t int_sqrt_uint64(uint64_t n)
{
    long double r = sqrt((long double)n);
    uint64_t x = (uint64_t)r;
    while ((x + 1) * (x + 1) <= n)
        ++x;
    while (x * x > n)
        --x;
    return x;
}

static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_prime_checked)
        return;
    if (primes_len == 0) {
        grow_primes_storage(1);
        primes[0] = 2;
        primes_len = 1;
        last_prime_checked = 2;
    }
    for (uint64_t cand = last_prime_checked + 1; cand <= limit; ++cand) {
        if ((cand & 1) == 0) {
            last_prime_checked = cand;
            continue;
        }
        bool is_prime = true;
        uint64_t root = int_sqrt_uint64(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= root; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            grow_primes_storage(primes_len + 1);
            primes[primes_len++] = cand;
        }
        last_prime_checked = cand;
    }
}

static void compute_d_sigma(uint64_t n, seq_t *d_out, seq_t *sigma_out)
{
    seq_t d     = (seq_t)1;
    seq_t sigma = (seq_t)1;
    uint64_t temp = n;
    uint64_t root = int_sqrt_uint64(temp);
    ensure_primes_up_to(root);
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
            seq_t p_seq = (seq_t)p;
            seq_t pow = (seq_t)1;
            for (int k = 0; k < exp + 1; ++k)
                pow *= p_seq;
            seq_t term = (pow - (seq_t)1) / (p_seq - (seq_t)1);
            sigma *= term;
        }
    }
    if (temp > 1) {
        d     *= (seq_t)2;
        sigma *= ((seq_t)temp + (seq_t)1);
    }
    *d_out     = d;
    *sigma_out = sigma;
}

static void ensure_cache(idx_t n)
{
    while (seq_cache_size < (size_t)n) {
        ++current_k;
        seq_t d, sigma;
        compute_d_sigma(current_k, &d, &sigma);
        if (sigma % d != (seq_t)0) {
            grow_seq_cache(seq_cache_size + 1);
            seq_cache[seq_cache_size++] = (seq_t)current_k;
        }
    }
}

/*** required public function ********************************/

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, but should not occur */
    ensure_cache(n);
    return seq_cache[n - 1];
}
