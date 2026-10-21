/*
 * A056273  Word structures of length n using a 6-ary alphabet.
 * https://oeis.org/A056273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..6} Stirling2(n,k). The values are built by a
 *            dynamic recurrence S(n,k) = k*S(n-1,k) + S(n-1,k-1) (k>=1), with
 *            S(0,0)=1, S(0,k)=0 for k>0. a(n) = Sum_{k=1..6} S(n,k); a(0)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */

static seq_t *a_vals  = NULL;      /* cached a(n) values   */
static size_t a_alloc = 0;         /* allocated size of a_vals */
static idx_t last_n   = (idx_t)-1;  /* highest n computed */

/* Last computed Stirling numbers S(n,k), k=0..6. */
static seq_t prev[7];
static seq_t curr[7];

seq_t a(idx_t n)
{
    /* Handle n==0 directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Initialise on first call. */
    if (a_vals == NULL) {
        a_alloc = 16;
        a_vals = (seq_t *)malloc(a_alloc * sizeof(seq_t));
        a_vals[0] = (seq_t)1;        /* a(0) = 1 */
        /* Initialise Stirling numbers for n = 0. */
        prev[0] = (seq_t)1;
        for (int k = 1; k <= 6; ++k) {
            prev[k] = (seq_t)0;
        }
        last_n   = 0;
    }

    /* Extend the cache if necessary. */
    if (n > last_n) {
        /* Grow allocation to at least n+1 elements. */
        size_t needed = (size_t)n + 1;
        if (needed > a_alloc) {
            size_t new_alloc = a_alloc;
            if (new_alloc < needed) new_alloc = needed;
            while (new_alloc < needed) new_alloc <<= 1;
            a_vals = (seq_t *)realloc(a_vals, new_alloc * sizeof(seq_t));
            a_alloc = new_alloc;
        }

        /* Compute a(n) incrementally up to n. */
        for (idx_t m = last_n + 1; m <= n; ++m) {
            curr[0] = (seq_t)0;                     /* S(m,0)=0 for m>0 */
            for (int k = 1; k <= 6; ++k) {
                curr[k] = prev[k] * (seq_t)k + prev[k - 1];
            }

            /* a(m) = sum_{k=1..6} curr[k] */
            seq_t sum = curr[1];
            for (int k = 2; k <= 6; ++k) {
                sum += curr[k];
            }
            a_vals[m] = sum;

            /* Prepare for next iteration. */
            for (int k = 0; k <= 6; ++k) {
                prev[k] = curr[k];
            }
        }
        last_n = n;
    }

    return a_vals[n];
}
