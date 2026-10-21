/*
 * A098051  Number of peakless Motzkin paths with no U H...HU's where U=(1,1)
 *          and H=(1,0) (can be easily expressed using RNA secondary structure
 *          terminology).
 * https://oeis.org/A098051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (from the OEIS entry):
   For n ≥ 7:
   (n+2)*a(n) = 5*(n+1)*a(n‑1) - (8*n+2)*a(n‑2) + 4*n*a(n‑3)
                + 2*(2*n‑11)*a(n‑5) - (8*n‑46)*a(n‑6) + 4*(n‑6)*a(n‑7)
   Initial terms:
   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=8, a(6)=16, a(7)=32.
   The function below memoises computed values for arbitrary n.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external integer types provided by the build system */
#ifndef seq_t
/* placeholder: the build will supply seq_t */
#endif
#ifndef idx_t
/* placeholder: the build will supply idx_t */
#endif

static const seq_t base_terms[8] = {1, 1, 1, 2, 4, 8, 16, 32};

/* Memoisation storage */
static seq_t* memo = NULL;        /* array of computed a(n) */
static size_t  memo_size = 0;     /* number of elements stored */

/* Function to compute a(n) */
seq_t a(idx_t n)
{
    /* base cases */
    if (n < 8) {
        return base_terms[(size_t)n];
    }

    /* initialise memoisation array on first call */
    if (memo == NULL) {
        memo_size = 8;                                  /* store 0..7 */
        memo = (seq_t*)malloc(memo_size * sizeof(seq_t));
        for (size_t i = 0; i < memo_size; ++i) {
            memo[i] = base_terms[i];
        }
    }

    /* extend memoisation array up to n */
    if ((size_t)n >= memo_size) {
        size_t old_size = memo_size;
        memo_size = (size_t)n + 1;                      /* include n */
        memo = (seq_t*)realloc(memo, memo_size * sizeof(seq_t));

        for (size_t i = old_size; i < memo_size; ++i) {
            /* compute a(i) using the recurrence */
            idx_t m = (idx_t)i;            /* current n in recurrence */

            /* positive contributions */
            seq_t pos = 5 * ((seq_t)(m + 1)) * memo[m - 1]    /* 5*(n+1)*a(n-1) */
                      + 4 * ((seq_t)m) * memo[m - 3]           /* 4*n*a(n-3) */
                      + 2 * ((seq_t)(2 * m - 11)) * memo[m - 5]/* 2*(2*n-11)*a(n-5) */
                      + 4 * ((seq_t)(m - 6)) * memo[m - 7];    /* 4*(n-6)*a(n-7) */

            /* negative contributions */
            seq_t neg = (8 * ((seq_t)m) + 2) * memo[m - 2]   /* (8*n+2)*a(n-2) */
                      + (8 * ((seq_t)m) - 46) * memo[m - 6]; /* (8*n-46)*a(n-6) */

            /* numerator = pos - neg (pos ≥ neg for n ≥ 7) */
            seq_t num = pos - neg;

            /* divide by (n+2) */
            memo[i] = num / ((seq_t)(m + 2));
        }
    }

    return memo[n];
}
