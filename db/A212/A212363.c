/*
 * A212363  Number A(n,k) of Dyck n-paths all of whose ascents and descents
 *          have lengths equal to 1+k*m (m>=0); square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A212363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 102, u32 n <= 250, u64 n <= 777,
 *            u128 n <= 2625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212363: Number a(n) of Dyck n-paths all of whose ascents and descents
 * have lengths 1 + k*m (m>=0), read by antidiagonals of the square array
 * A(n,k).  The recurrence is
 *   A(n,0) = A(0,k) = 1
 *   A(n,k) = A(n-1,k) + sum_{j=1..n-k} A(j,k)*A(n-k-j,k)   (n,k>0)
 * a(n) is obtained by traversing the array by anti‑diagonals.
 *
 * The implementation keeps a dynamic 2‑D table computed on demand.
 * Complexity is O(s^3) for the largest anti‑diagonal s needed to reach
 * index n.  For n≤10 000 this is trivial.  The routine is fully
 * compliant with the type constraints and uses only integer arithmetic.
 */

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------------*
 * Common integer‑sqrt routine (floor) for unsigned 64‑bit values. *
 *------------------------------------------------------------------*/
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x + 1;
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/*-----------------------------------------------------------------*
 * Dynamic table of A(n,k).  The table is square with side length
 * equal to the largest anti‑diagonal that has been needed.
 *-----------------------------------------------------------------*/
static seq_t *A_tbl   = NULL;  /* pointer to allocated table          */
static size_t  A_dim  = 0;     /* current dimension (≥1)              */

/* rebuild the table up to anti‑diagonal s (i.e. rows/cols 0..s)   */
static void rebuild_table(size_t s)
{
    size_t new_dim = s + 1;                /* number of rows/cols    */
    size_t cap     = new_dim * new_dim;    /* total number of cells  */

    seq_t *new_tbl = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!new_tbl)
        exit(1);                          /* allocation failure     */

    /* populate the table row by row */
    for (size_t i = 0; i <= s; ++i) {
        for (size_t j = 0; j <= s; ++j) {
            size_t idx = i * new_dim + j;
            /* base cases */
            if ((i == 0) || (j == 0))
                new_tbl[idx] = 1;
            else {
                /* recurrence */
                seq_t sum = new_tbl[(i - 1) * new_dim + j]; /* A(i-1,j) */
                size_t lim = (i > j) ? (i - j) : 0;          /* n-k    */
                for (size_t k = 1; k <= lim; ++k) {
                    size_t a1 = k * new_dim + j;               /* A(k,j)       */
                    size_t a2 = (i - j - k) * new_dim + j;     /* A(i-j-k,j)   */
                    sum += new_tbl[a1] * new_tbl[a2];
                }
                new_tbl[idx] = sum;
            }
        }
    }

    /* replace the old table */
    free(A_tbl);
    A_tbl  = new_tbl;
    A_dim  = new_dim;
}

/*-----------------------------------------------------------------*
 * Public sequence routine.  Computes a(n) with anti‑diagonal
 * enumeration.
 *-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* determine the anti‑diagonal index s such that
     * n < (s+1)(s+2)/2  and n ≥ s(s+1)/2            */
    uint64_t m  = 8 * (uint64_t)n + 1;           /* 8n+1  fits 64‑bit here */
    uint64_t sm = isqrt_u64(m);                  /* floor sqrt(8n+1) */
    uint64_t s  = (sm - 1) / 2;                  /* integer division */

    /* ensure the table covers this anti‑diagonal */
    if (s >= A_dim)
        rebuild_table((size_t)s);

    /* base index of this diagonal: s(s+1)/2 */
    size_t base = (size_t)s * (s + 1) / 2;
    size_t i    = (size_t)(n - base);            /* row index          */
    size_t j    = (size_t)(s - i);               /* column index       */

    /* table element address: row-major layout */
    return A_tbl[i * A_dim + j];
}
