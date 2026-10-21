/*
 * A214747  Least m>0 such that n-m divides n!!+m.
 * https://oeis.org/A214747
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - d, where d is the largest divisor of n!! + n with 1 ≤
 *            d < n. Equivalently, a(n) is the smallest m > 0 such that (n-m)
 *            | (n!! + m)
 * Range:     u8 n <= 268, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                  /* sequence starts at offset 2 */

    /* Allocate an array prod[d] for d = 1..n-1.
       prod[d] will hold n!! mod d. */
    size_t len = (size_t)n;               /* we need indices 0..n-1, but 0 unused */
    seq_t *prod = (seq_t *)malloc(len * sizeof(seq_t));
    if (!prod) return 0;                  /* allocation failure */

    for (uint64_t d = 1; d < (uint64_t)n; ++d) prod[d] = 1;

    /* Compute n!! modulo every d concurrently. */
    if ((n & 1) == 0) {                   /* n is even: multiply even numbers */
        for (uint64_t k = 2; k <= (uint64_t)n; k += 2) {
            for (uint64_t d = 1; d < (uint64_t)n; ++d) {
                uint64_t tmp = ((uint64_t)prod[d] * k) % d;
                prod[d] = (seq_t)tmp;
            }
        }
    } else {                              /* n is odd: multiply odd numbers */
        for (uint64_t k = 1; k <= (uint64_t)n; k += 2) {
            for (uint64_t d = 1; d < (uint64_t)n; ++d) {
                uint64_t tmp = ((uint64_t)prod[d] * k) % d;
                prod[d] = (seq_t)tmp;
            }
        }
    }

    /* Find the largest d < n that divides n!! + n. */
    for (uint64_t d = (uint64_t)n - 1; d >= 1; --d) {
        uint64_t val = ((uint64_t)prod[d] + (uint64_t)n) % d;
        if (val == 0) {
            seq_t m = (seq_t)(n - d);
            free(prod);
            return m;
        }
        if (d == 1) break;                 /* avoid unsigned underflow */
    }

    free(prod);
    return 0;                              /* should never reach here */
}
