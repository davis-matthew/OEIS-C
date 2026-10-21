/*
 * A072049  a(n) = floor(2^(n/(floor(n*log(2)/log(prime(n)))))).
 * https://oeis.org/A072049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2^( n / floor( n*log(2)/log(prime(n)) ) ) )
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"        /* seq_t big_pow(seq_t base, seq_t exp); */

/* --------------------------------------------------------------------- */
/* Prime generation: sieve up to a fixed limit (200000 covers the 10000th prime) */
static bool primes_inited = false;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_inited)
        return;

    const size_t LIMIT = 200000;

    /* Allocate a boolean array for the sieve */
    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime)
        exit(1); /* allocation failure */

    for (size_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++primes_cnt;

    /* Allocate array of primes */
    primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes)
        exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;

    free(is_prime);
    primes_inited = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    init_primes();

    /* Protect against out‑of‑range requests */
    if ((size_t)(n - 1) >= primes_cnt)
        return (seq_t)0;

    uint64_t prime_n = primes[(size_t)(n - 1)];
    seq_t p = (seq_t)prime_n;

    /* Compute 2^n once */
    seq_t pow2_n = big_pow((seq_t)2, (seq_t)n);

    /* Find b = floor(log_p (2^n))  (largest integer with p^b <= 2^n) */
    idx_t low_b = 1, high_b = n, best_b = 1;
    while (low_b <= high_b) {
        idx_t mid_b = low_b + (high_b - low_b) / 2;
        seq_t p_mid = big_pow(p, (seq_t)mid_b);
        if (p_mid <= pow2_n) {
            best_b = mid_b;
            low_b = mid_b + 1;
        } else {
            if (mid_b == 0) break;
            high_b = mid_b - 1;
        }
    }
    idx_t b = best_b;

    /* Compute largest integer m such that m^b <= 2^n (i.e. floor_root_b(2^n)) */
    idx_t exp_high = (n + b - 1) / b;                    /* ceil(n/b) */
    seq_t high_m = big_pow((seq_t)2, (seq_t)exp_high);

    seq_t low_m  = (seq_t)1;
    seq_t best_m = low_m;

    while (low_m <= high_m) {
        seq_t mid = (low_m + high_m) >> 1;
        seq_t mid_pow = big_pow(mid, (seq_t)b);
        if (mid_pow <= pow2_n) {
            best_m = mid;
            low_m = mid + 1;
        } else {
            if (mid == 0) break;      /* safety, though mid>0 always */
            high_m = mid - 1;
        }
    }

    return best_m;
}
