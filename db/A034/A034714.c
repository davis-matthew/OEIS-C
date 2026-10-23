/*
 * A034714  Dirichlet convolution of squares with themselves.
 * https://oeis.org/A034714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^2 * tau(n)   (tau(n) = number of divisors of n)
 */
#include <stdlib.h>
#include <stdint.h>

/*  n is 1‑based */
seq_t a(idx_t n)
{
    /* Defensive: if called with 0, return 0 */
    if (n == 0) return 0;

    /* Static cache for divisor counts */
    static uint64_t *divcnt = NULL;   /* 1‑based array: divcnt[i] = tau(i) */
    static idx_t    cache_n = 0;      /* largest n currently cached */

    if (n > cache_n) {
        /* Need to extend cache up to n */
        /* Allocate new array of size n+1, initialized to 0 */
        uint64_t *new_arr = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
        if (!new_arr) return 0;  /* allocation failure, return 0 */

        /* Compute divisor counts up to n */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                ++new_arr[j];
        }

        /* Replace old cache */
        free(divcnt);
        divcnt = new_arr;
        cache_n = n;
    }

    /* Compute a(n) = n^2 * tau(n) */
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn * (seq_t)divcnt[n];
    return result;
}
