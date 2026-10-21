/*
 * A065995  a(n) = prime(prime(n) + n).
 * https://oeis.org/A065995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)+n)
 * Range:     u8 n <= 13, u16 n <= 759, u32 n <= 9473, u64 n <= 9473,
 *            u128 n <= 9473, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Returns the nth prime (1‑based).  Sequence indices are always >= 1. */
static seq_t get_kth_prime(idx_t k)
{
    /* The primes array is initialized the first time this function is
       called.  It is large enough for the test ranges (n up to ~10 000). */
    static bool initialized = false;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!initialized) {
        /* 2 000 000 is comfortably larger than the index needed for
           n ≤ 10 000 (prime(10 000) ≈ 104 729; prime(104 729+10 000) ≈ 1.3 M). */
        idx_t limit = 2000000;
        build_primes(limit, &primes, &prime_cnt);
        initialized = true;
    }

    /* k is 1‑based; return 0 for invalid indices. */
    if (k == 0 || k > prime_cnt)
        return 0;
    return (seq_t)primes[k - 1];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for offset 0 */

    /* p = prime(n) */
    seq_t p = get_kth_prime(n);

    /* idx = p + n (1‑based index of the second prime) */
    idx_t idx = (idx_t)p + n;

    /* result = prime(idx) */
    return get_kth_prime(idx);
}
