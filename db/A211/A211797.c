/*
 * A211797  Number of 4-tuples (w,x,y,z) with all terms in {1,...,n} and
 *          w*x>2*y*z.
 * https://oeis.org/A211797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and w*x
 *            > 2*y*z. Computation by counting all pairs (y,z), building a
 *            prefix count of products, then summing for all (w,x).
 *            Complexity: O(n²) time, O(n²) memory
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return 0;

    /* Size of the array needed */
    size_t prod_size = (size_t)n * (size_t)n + 1; /* +1 for index 0 */
    seq_t *prod = (seq_t *)malloc(prod_size * sizeof(seq_t));
    if (!prod) return 0; /* allocation failure, return 0 as safe fallback */

    /* initialise to zero */
    for (size_t i = 0; i < prod_size; ++i) prod[i] = 0;

    /* Count products y*z for 1 ≤ y,z ≤ n */
    for (idx_t y = 1; y <= n; ++y) {
        for (idx_t z = 1; z <= n; ++z) {
            size_t p = (size_t)y * (size_t)z;
            prod[p] += 1;
        }
    }

    /* Transform count array into prefix sums: prod[t] = number of pairs (y,z) with y*z ≤ t */
    seq_t acc = 0;
    for (size_t t = 0; t < prod_size; ++t) {
        acc += prod[t];
        prod[t] = acc;
    }

    /* Now sum over all (w,x) the count of (y,z) with w*x > 2*y*z */
    seq_t result = 0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            seq_t wx = (seq_t)w * (seq_t)x;
            seq_t t   = (wx - 1) / 2;          /* floor((w*x-1)/2) */
            result   += prod[(size_t)t];
        }
    }

    free(prod);
    return result;
}
