/*
 * A145551  Numbers k such that product of divisors of k / sum of divisors of
 *          k is an integer.
 * https://oeis.org/A145551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145551:
 * a(n) is the n-th positive integer k such that the product of divisors of k
 * divided by the sum of divisors of k is an integer.
 * Equivalently k^τ(k) is divisible by σ(k)^2.
 * (τ = number of divisors, σ = sum of divisors.)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Prime sieve ----------------------------------------------------- */
#define MAXP 200000                     /* enough for all required k */

static bool prime_marked[MAXP + 1];
static uint32_t primes[MAXP];
static size_t primes_cnt;
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done) return;
    for (size_t i = 2; i * i <= MAXP; ++i)
        if (!prime_marked[i])
            for (size_t j = i * i; j <= MAXP; j += i)
                prime_marked[j] = true;

    primes_cnt = 0;
    for (size_t i = 2; i <= MAXP; ++i)
        if (!prime_marked[i])
            primes[primes_cnt++] = (uint32_t)i;
    sieve_done = true;
}

/* Factor a number n (n <= uint64_t_max) using the prime list.
 * The prime factors and their exponents are stored in the provided arrays.
 * The arrays must be large enough to hold all factors (max 64 for 64-bit n).
 */
static void factor_uint64(uint64_t n, uint32_t *pf, int *pe, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[(*cnt)++] = p;
            pe[(*cnt) - 1] = e;
        }
    }
    if (n > 1) {
        pf[(*cnt)++] = (uint32_t)n;
        pe[(*cnt) - 1] = 1;
    }
}

/* Add exponent 'exp' of prime 'p' to the factor list fact[]. */
static void add_factor(uint32_t *pf, int *pe, size_t *cnt, uint32_t p, int exp)
{
    if (exp == 0) return;
    for (size_t i = 0; i < *cnt; ++i)
        if (pf[i] == p) {
            pe[i] += exp;
            return;
        }
    pf[(*cnt)++] = p;
    pe[(*cnt) - 1] = exp;
}

/* Test whether k satisfies the property. */
static bool good_k(uint64_t k)
{
    if (k == 1) return true;

    init_sieve();

    /* factor k */
    uint32_t kpf[64];
    int kpe[64];
    size_t kcnt;
    factor_uint64(k, kpf, kpe, &kcnt);

    /* tau(k) */
    seq_t tau = 1;
    for (size_t i = 0; i < kcnt; ++i)
        tau *= (seq_t)(kpe[i] + 1);

    /* factor sigma(k) */
    uint32_t sf[128];
    int spe[128];
    size_t scnt = 0;

    for (size_t i = 0; i < kcnt; ++i) {
        uint32_t p = kpf[i];
        int e = kpe[i];

        /* compute sigma(p^e) = 1 + p + … + p^e */
        uint64_t sum = 1;
        uint64_t power = 1;
        for (int j = 0; j < e; ++j) {
            power *= p;
            sum += power;
        }

        /* factor the sum */
        uint32_t sf_tmp[64];
        int sf_exp[64];
        size_t sfcnt_tmp;
        factor_uint64(sum, sf_tmp, sf_exp, &sfcnt_tmp);

        for (size_t j = 0; j < sfcnt_tmp; ++j)
            add_factor(sf, spe, &scnt, sf_tmp[j], sf_exp[j]);
    }

    /* check conditions */
    for (size_t i = 0; i < scnt; ++i) {
        uint32_t q = sf[i];
        /* exponent of q in k */
        int k_exp_q = 0;
        for (size_t j = 0; j < kcnt; ++j)
            if (kpf[j] == q) { k_exp_q = kpe[j]; break; }
        if (k_exp_q == 0) return false; /* new prime factor */

        /* left exponent: e_q * tau(k) */
        seq_t left = (seq_t)k_exp_q * tau;
        /* required exponent: 2 * exp_sigma(q) */
        seq_t required = (seq_t)(2 * spe[i]);

        if (left < required) return false;
    }
    return true;
}

/* --- Cache for sequence values ------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t checked_up_to = 1;   /* next number to test */

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but never called for n<1 */

    /* initialize cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {
            /* first term is 1 */
            cache[cache_len++] = (seq_t)1;
            checked_up_to = 2;
            continue;
        }

        /* search for next good k */
        uint64_t k = checked_up_to;
        while (!good_k(k)) ++k;

        /* grow cache if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* out of memory – problematic, but tiny chance here */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)k;
        checked_up_to = k + 1;
    }

    return cache[n - 1];
}
