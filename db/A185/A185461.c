/*
 * A185461  Number of (n+1) X 4 binary arrays with no 2 X 2 subblock
 *          determinant equal to any horizontal or vertical neighbor 2 X 2
 *          subblock determinant.
 * https://oeis.org/A185461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A185461: Number of (n+1) × 4 binary arrays with the 2×2 subblock determinant
 *          condition.  Empirical linear recurrence of order 6 for n > 12:
 *
 * a(1) =  92
 * a(2) = 240
 * a(3) = 866
 * a(4) = 2766
 * a(5) = 8762
 * a(6) = 27102
 * a(7) = 83688
 * a(8) = 258056
 * a(9) = 795746
 * a(10)= 2453840
 * a(11)= 7566872
 * a(12)= 23333794
 *
 * For n > 12:
 *   a(n) = 2*a(n‑1) + 2*a(n‑2) + 3*a(n‑3) + 2*a(n‑4) + 4*a(n‑5) + 2*a(n‑6).
 *
 * The function below memoises values so that any order of calls is handled in
 * O(n) time, with n the largest argument seen so far.  All arithmetic is
 * performed in seq_t, meeting the integer type constraints of the build.
 */

#include <stdint.h>
#include <stdlib.h>

/* static table of the first 12 terms (index 0 unused)             */
static const seq_t init_terms[13] = {
    0ULL,   /* index 0 unused */
    92ULL, 240ULL, 866ULL, 2766ULL, 8762ULL,
    27102ULL, 83688ULL, 258056ULL, 795746ULL,
    2453840ULL, 7566872ULL, 23333794ULL
};

/* ------------------------------------------------------------------ */
/* Memoised sequence generation                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static array holds computed values; memo[i] == a(i) for i>=1          */
    static seq_t *memo   = NULL;   /* allocated lazily */
    static idx_t memo_sz = 0;      /* current allocating size (elements) */

    if (n < 1) {
        /* Undefined for n<1; returning 0 for safety */
        return (seq_t)0;
    }

    /* initialise memo on first call or enlarge when needed                */
    if (memo == NULL) {
        /* allocate space for indices 0..12 (0 unused)                      */
        memo_sz = 13;
        memo    = (seq_t *)calloc(memo_sz, sizeof(seq_t));
        if (memo == NULL) { /* allocation failure; return 0 as fallback  */
            return (seq_t)0;
        }
        /* copy the first 12 known terms */
        for (idx_t i = 1; i <= 12; ++i) {
            memo[i] = init_terms[i];
        }
    } else if (memo_sz <= n) {
        /* Increase allocation to hold all terms up to n                      */
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_sz * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure; return 0 as fallback                    */
            return (seq_t)0;
        }
        memo = tmp;
        /* initialise any newly added positions to zero                     */
        for (idx_t i = memo_sz; i <= n; ++i) {
            memo[i] = (seq_t)0;
        }
        memo_sz = new_sz;
    }

    if (n <= 12) {
        return memo[n];                 /* already initialised              */
    }

    /* compute sequence from the last stored value up to n                 */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t v =  2ULL * memo[i - 1]
                + 2ULL * memo[i - 2]
                + 3ULL * memo[i - 3]
                + 2ULL * memo[i - 4]
                + 4ULL * memo[i - 5]
                + 2ULL * memo[i - 6];
        memo[i] = v;
    }

    return memo[n];
}
