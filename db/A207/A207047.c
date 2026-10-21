/*
 * A207047  Number of (n+1) X 6 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 34, u64 n <= 82, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 4*a(n-3) for n > 13; 
   the first 13 values are given explicitly below. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *A = NULL;     /* cache of computed values */
static size_t ACAP = 0;     /* allocated capacity (number of elements) */
static idx_t ALAST = 0;     /* highest index that has been computed */

/* Return the n-th term of A207047.  n ≥ 1; behaviour for n < 1 is arbitrary. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined offset below 1 */

    /* Ensure the cache can hold index n. */
    if (A == NULL || ACAP <= (size_t)n) {
        size_t newcap = ACAP ? ACAP * 2 : 64;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        A = tmp;
        ACAP = newcap;
    }

    /* Store the first 13 terms on first use. */
    if (ALAST < 13) {
        static const unsigned long long init_vals[14] = {
            0ULL,               /* dummy for index 0 */
            4371ULL, 2244ULL, 2094ULL, 3408ULL, 4944ULL, 7584ULL,
            13752ULL, 20208ULL, 31158ULL, 55920ULL, 81936ULL,
            125208ULL, 223776ULL
        };
        size_t i;
        for (i = 1; i <= 13 && i < ACAP; ++i)
            A[i] = (seq_t)init_vals[i];
        if (ALAST < 13) ALAST = 13;
    }

    if ((size_t)n <= (size_t)ALAST) return A[n];

    /* Compute missing terms using the recurrence a(n)=4*a(n-3). */
    size_t i;
    for (i = (size_t)(ALAST + 1); i <= (size_t)n; ++i)
        A[i] = (seq_t)4 * A[i - 3];
    ALAST = n;

    return A[n];
}
