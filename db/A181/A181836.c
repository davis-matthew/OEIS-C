/*
 * A181836  The product of primes <= n that are strongly prime to n.
 * https://oeis.org/A181836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   product of primes ≤ n that are strongly prime to n. * A prime p
 *            is strongly prime to n iff p is coprime to n * and p does not
 *            divide n-1
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

static uint64_t *prime_arr = NULL;      /* array of primes up to current limit */
static size_t      prime_cnt = 0;       /* number of primes in prime_arr      */
static idx_t      prime_limit = 0;      /* highest n for which primes were built */

/* Compute a(n) – product of primes ≤ n that are strongly prime to n */
seq_t a(idx_t n)
{
    /* Build primes up to n if necessary */
    if ((idx_t)n > prime_limit) {
        free(prime_arr);
        build_primes_upto((idx_t)n, &prime_arr, &prime_cnt);
        prime_limit = (idx_t)n;
    }

    seq_t product = 1;  /* identity element in multiplicative semigroup */

    /* Iterate over all primes ≤ n */
    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= n; ++i) {
        uint64_t p = prime_arr[i];
        /* n is idx_t (unsigned).  Use modulo to test divisibility. */
        if ((n % (idx_t)p) != 0 && ((n - 1) % (idx_t)p) != 0) {
            product = seq_mul(product, (seq_t)p);
        }
    }

    return product;
}
