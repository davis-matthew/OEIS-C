/*
 * A233296  a(n) = |{0 < k < n: k*prime(n-k) + 1 is prime}|.
 * https://oeis.org/A233296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of k with 0 < k < n such that k*prime(n‑k)+1 is prime
 * Range:     u8 n <= 19, u16 n <= 1099, u32 n <= 1099, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a cache of primes up to a fixed limit and
 * then scans all k for each call. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* declares build_primes_upto */
#include "../utils/is_prime_big.h"        /* declares is_prime_big */

static uint64_t *primes = NULL;   /* array of primes up to the desired limit */
static size_t   prime_cnt = 0;    /* number of primes stored */
static bool     primes_ready = false;

/* Build a list of primes sufficient for all n ≤ 10000.
 * 200 000 is comfortably above the 10000th prime (104729). */
static void init_primes(void)
{
    if (!primes_ready) {
        size_t cnt;
        build_primes_upto(200000ULL, &primes, &cnt);
        prime_cnt = cnt;
        primes_ready = true;
    }
}

/* a(n) = |{ 0 < k < n : k * prime(n-k) + 1 is prime }|   */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 by definition */
        return (seq_t)0;

    init_primes();

    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t diff = n - k;                 /* n-k in the sense of the definition */
        seq_t p  = (seq_t)primes[diff - 1]; /* prime_(n-k) */
        seq_t val = (seq_t)k * p + 1;        /* k*prime(n-k) + 1 */
        if (is_prime_big(val))
            ++result;
    }
    return result;
}
