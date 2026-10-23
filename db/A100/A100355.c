/*
 * A100355  Maximal number of 15432 patterns in a permutation of 1,2,...,n.
 * https://oeis.org/A100355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 <= k < n} ( a(k) + k * binom(n-k, 4) )
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A100355 – maximal number of 15432 patterns in a permutation of 1..n  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom4.h"

static seq_t *A = NULL;      /* DP array, 1‑based indexing */
static idx_t  max_computed = 0;

/*  Derives a( n )  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used by OEIS definition */

    /* grow the DP array if necessary   */
    if (max_computed < n) {
        idx_t new_cap = max_computed > 1 ? max_computed : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failure */
        A = tmp;

        /* compute new values in increasing order   */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {           /* a(1) = 0 */
                A[i] = 0;
                continue;
            }
            seq_t best = 0;
            for (idx_t k = 1; k < i; ++k) {
                idx_t m = i - k;    /* n-k  */
                if (m < 4) continue;   /* binom4(m) would be 0 */
                seq_t term = A[k] + (seq_t)k * binom4((seq_t)m);
                if (term > best) best = term;
            }
            A[i] = best;
        }
        max_computed = n;
    }
    return A[n];
}
