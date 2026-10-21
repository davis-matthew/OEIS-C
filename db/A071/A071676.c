/*
 * A071676  Array read by antidiagonals of signed variant of trinomial
 *          coefficients with T(n,k)=T(n-1,k)+T(n-1,k-1)-T(n-1,k-2) starting
 *          with T(0,0)=1.
 * https://oeis.org/A071676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i, j) where T(0,0)=1 and * T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) − T(n-1,k-2). * The pairs (i,j) satisfy n = i*(i+1)/2
 *            + j and 0 ≤ j ≤ i
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  includes and helper prototypes ---------------------------------------------------- */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdint.h>   /* for uintptr_t, if needed */

/*  integer square‑root for unsigned 64‑bit values ------------------------------------- */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (x >> 1) + 1;   /* high is at least sqrt(x) */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && x / mid < mid)        /* mid*mid > x   */
            high = mid - 1;
        else
            low  = mid + 1;
    }
    return high;
}

/*  static cache for trinomial coefficients ------------------------------------------- */
static seq_t **T_table = NULL;        /* T_table[row][col] */
static idx_t  T_max_s = (idx_t)-1;     /* largest row/col that has been computed */

/*  Extend the cache up to (including) new_s ------------------------------------------- */
static void build_table(idx_t new_s)
{
    /* Allocate new 2‑D array */
    seq_t **newT = (seq_t **)malloc((new_s + 1) * sizeof(seq_t *));
    if (!newT) return;   /* allocation failure – the caller will get a zero result */
    for (idx_t r = 0; r <= new_s; ++r) {
        newT[r] = (seq_t *)calloc((new_s + 1), sizeof(seq_t));
        if (!newT[r]) { /* cleanup previously allocated rows */
            for (idx_t rr = 0; rr < r; ++rr) free(newT[rr]);
            free(newT);
            return;
        }
    }

    /* Base case */
    newT[0][0] = (seq_t)1;

    /* Fill by recurrence   T(n,k)=T(n-1,k)+T(n-1,k-1)-T(n-1,k-2)    */
    for (idx_t n = 1; n <= new_s; ++n) {
        for (idx_t k = 0; k <= new_s; ++k) {
            seq_t a = newT[n - 1][k];
            seq_t b = (k > 0) ? newT[n - 1][k - 1] : (seq_t)0;
            seq_t c = (k > 1) ? newT[n - 1][k - 2] : (seq_t)0;
            newT[n][k] = a + b - c;
        }
    }

    /* Free the old table, if any */
    if (T_table) {
        for (idx_t r = 0; r <= T_max_s; ++r) free(T_table[r]);
        free(T_table);
    }

    /* Install the new table */
    T_table  = newT;
    T_max_s  = new_s;
}

/*  main sequence function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the antidiagonal index s:  n = s*(s+1)/2 + j   with 0 ≤ j ≤ s.   */
    idx_t s = (floor_isqrt_u64(8 * n + 1) - 1) / 2;

    /* Make sure the table contains all necessary rows/cols. */
    if (T_table == NULL || s > T_max_s) build_table(s);

    /* Compute the position inside the diagonal. */
    idx_t tri  = (s * (s + 1)) / 2;          /* safe in 64 bit for all allowed n */
    idx_t i    = n - tri;                    /* 0 ≤ i ≤ s   */
    idx_t j    = s - i;                      /* 0 ≤ j ≤ s   */

    /* Return the stored signed trinomial coefficient. */
    return T_table[i][j];
}
