/*
 * A086849  Sum of first n nonsquares.
 * https://oeis.org/A086849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (i + floor(1/2 + sqrt(i)))
 * Range:     u8 n <= 19, u16 n <= 349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t *prefix = NULL;   /* cumulative sums, prefix[0] = 0 */
static idx_t prefix_max = 0;   /* maximum n computed so far */

/* Compute the n-th term of A086849 */
seq_t a(idx_t n)
{
    /* Ensure we have computed up to n */
    if (n > prefix_max) {
        /* Allocate or grow the array */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_prefix = (seq_t *)realloc(prefix, new_size * sizeof(seq_t));
        if (!new_prefix) {
            /* Out of memory; return 0 as a safe fallback */
            return (seq_t)0;
        }
        prefix = new_prefix;

        /* Initialize new entries */
        for (idx_t i = prefix_max + 1; i <= n; ++i) {
            seq_t idx = (seq_t)i;
            seq_t sq = floor_sqrt(idx);          /* floor(sqrt(i)) */
            seq_t round_sqrt;
            /* Determine round(sqrt(i)) using integer arithmetic */
            if (idx <= sq * sq + sq)
                round_sqrt = sq;
            else
                round_sqrt = sq + 1;
            seq_t val = idx + round_sqrt;        /* A000037(i) */
            prefix[i] = prefix[i - 1] + val;     /* cumulative sum */
        }
        prefix_max = n;
    }
    return prefix[n];
}
