/*
 * A100354  Maximal number of 1432 patterns in a permutation of 1,2,...,n.
 * https://oeis.org/A100354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1<=k<n} ( a(k) + k*choose(n-k,3) ) where choose(t,3)
 *            = t*(t-1)*(t-2)/6 (0 if t<3)
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=0 */

#include <stdlib.h>
#include "../utils/mem.h"

/* helper: binomial coefficient C(t,3) for t>=0, returns 0 if t<3 */
static seq_t
choose3(seq_t t)
{
    if (t < 3) return (seq_t)0;
    return (t * (t - 1) * (t - 2)) / 6;
}

static seq_t *A = NULL;     /* memoized array of previous values */
static idx_t  A_size = 0;   /* allocated size (highest index+1) */

seq_t a(idx_t n)
{
    /* ensure array big enough to hold indices up to n */
    if (A_size == 0) {
        A = mem_new(seq_t, (size_t)2); /* indices 0 and 1 used */
        if (!A) return (seq_t)0;     /* allocation failure – return 0 */
        A[1] = (seq_t)0;             /* a(1) = 0 */
        A_size = 2;
    }

    if (n + 1 > A_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, A, new_size);
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        A = tmp;
        /* compute new entries from old size to n */
        for (idx_t i = A_size; i <= n; ++i) {
            if (i == 1) {
                A[1] = (seq_t)0;
                continue;
            }
            seq_t best = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t t = (seq_t)(i - k);                     /* n-k */
                seq_t term = A[k] + (k * t * (t - 1) * (t - 2)) / 6;
                if (term > best)
                    best = term;
            }
            A[i] = best;
        }
        A_size = new_size;
    }

    return A[n];
}
