/*
 * A009224  Expansion of exp(sinh(x))*x.
 * https://oeis.org/A009224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * c_{n-1}, * where c_0 = 1 * and for m ≥ 1: * c_m =
 *            Σ_{j = 0 .. m-1, j even} C(m-1, j) * c_{m-1-j} * (equivalently,
 *            c_m = Σ_{odd k ≤ m} C(m-1, k-1) * c_{m-k}) * * This is the
 *            coefficient sequence of exp(sinh(x)) * x
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *c = NULL;        /* memoised c_m values, indexed 0 … N-1 */
static size_t cache_size = 0;  /* number of elements currently stored */

/* Compute a(n) for any n ≥ 0.  The result is cached so that calls
 * in arbitrary order run in total O(N²) time for the largest N requested. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is defined as 0 */

    /* Ensure we have c[0 … n-1] available. */
    if (cache_size < (size_t)n) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(c, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe value.  */
            free(c);
            c = NULL;
            cache_size = 0;
            return 0;
        }
        /* Keep the old data and extend.  */
        c = tmp;

        /* Compute missing c_m values from previous size up to new_size-1. */
        for (size_t m = cache_size; m < new_size; ++m) {
            if (m == 0) {
                c[0] = 1;            /* base case */
                continue;
            }
            seq_t sum = 0;
            size_t mm = m - 1;      /* n-1 in the recurrence */
            seq_t binom = 1;        /* C(mm, 0) */

            for (size_t j = 0; j <= mm; ++j) {
                if ((j & 1) == 0) { /* j even => k = j+1 odd */
                    sum += binom * c[mm - j];
                }
                if (j < mm) {
                    /* Update binom to C(mm, j+1) using integer arithmetic. */
                    seq_t num = (seq_t)(mm - j);
                    seq_t den = (seq_t)(j + 1);
                    binom = (binom * num) / den;
                }
            }
            c[m] = sum;
        }
        cache_size = new_size;
    }

    /* a(n) = n * c[n-1] */
    return ((seq_t)n) * c[n - 1];
}
