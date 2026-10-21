/*
 * A078475  Determinant of rank n matrix of 1..n^2 filled successively back
 *          and forth along antidiagonals.
 * https://oeis.org/A078475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -9*a(n-2) - 36*a(n-4) - 84*a(n-6) - 126*a(n-8) * -
 *            126*a(n-10) - 84*a(n-12) - 36*a(n-14) - 9*a(n-16) - a(n-18); *
 *            a(1..18) are given below
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 22, i64 n <= 338, i128 n <= 1450,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS sequence A078475.
 * The sequence is defined by the above recurrence for n > 18.
 * a(1) = 1, a(2) = -2, a(3) = 15, a(4) = -594, a(5) = -5187, a(6) = 23244,
 * a(7) = 122475, a(8) = -279292, a(9) = -1157143, a(10) = 1850930,
 * a(11) = 6642839, a(12) = -8529278, a(13) = -27810555, a(14) = 30741424,
 * a(15) = 93575187, a(16) = -92784984, a(17) = -268191855, a(18) = 244875462.
 */

#include <stdlib.h>

/* Pre‑computed initial values a(1)..a(18). Index 0 is unused. */
static const seq_t init_vals[19] = {
    0,                       /* unused for a(0) */
    (seq_t)1,               /* a(1)  */
    (seq_t)-2,              /* a(2)  */
    (seq_t)15,              /* a(3)  */
    (seq_t)-594,            /* a(4)  */
    (seq_t)-5187,           /* a(5)  */
    (seq_t)23244,           /* a(6)  */
    (seq_t)122475,          /* a(7)  */
    (seq_t)-279292,         /* a(8)  */
    (seq_t)-1157143,        /* a(9)  */
    (seq_t)1850930,         /* a(10) */
    (seq_t)6642839,         /* a(11) */
    (seq_t)-8529278,        /* a(12) */
    (seq_t)-27810555,       /* a(13) */
    (seq_t)30741424,        /* a(14) */
    (seq_t)93575187,        /* a(15) */
    (seq_t)-92784984,       /* a(16) */
    (seq_t)-268191855,      /* a(17) */
    (seq_t)244875462        /* a(18) */
};

/* Coefficients for the recurrence, ordered by offset 2,4,6,...,18. */
static const seq_t coeffs[9] = {
    (seq_t)-9, (seq_t)-36, (seq_t)-84, (seq_t)-126,
    (seq_t)-126, (seq_t)-84, (seq_t)-36, (seq_t)-9, (seq_t)-1
};

/* Cached values of the sequence.  memo[0] is unused. */
static seq_t *memo = NULL;
static idx_t memo_cap = 0;

/* Compute a(n).  The function is thread‑safe only if called from a
 * single thread; it uses static storage for memoisation. */
seq_t a(idx_t n)
{
    /* Initialisation: allocate space for a(1)..a(18) and fill them. */
    if (memo == NULL) {
        memo_cap = 19;                 /* capacity for indices 0..18 */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (memo == NULL) return 0;    /* allocation failure */
        for (idx_t i = 0; i < memo_cap; ++i)
            memo[i] = init_vals[i];
    }

    /* If n is within the cached range, return it. */
    if (n < memo_cap)
        return memo[n];

    /* Extend the cache up to n.  The new size is n+1 (indices 0..n). */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (tmp == NULL) return 0;          /* allocation failure */
    memo = tmp;
    /* Compute missing terms using the recurrence. */
    for (idx_t i = memo_cap; i <= n; ++i) {
        /* For i <= 18 the values are already set.  The loop starts at i=19. */
        if (i <= 18) continue;
        seq_t sum = 0;
        for (int k = 0; k < 9; ++k) {
            idx_t idx = i - 2 * (k + 1);  /* index of the previous term */
            sum += coeffs[k] * memo[idx];
        }
        memo[i] = sum;
    }
    memo_cap = new_cap;

    return memo[n];
}
