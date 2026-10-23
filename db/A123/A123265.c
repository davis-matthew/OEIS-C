/*
 * A123265  Fibonacci-Lucas triangle read by rows.
 * https://oeis.org/A123265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n,k)=T(n-1,k-1)+T(n-1,k)+T(n-2,k)  */
/* with T(0,0)=2, T(1,0)=1, T(n,k)=0 if k<0 or k>n.  */
/* a(n) is the linear enumeration of T(n,k) by rows, i.e. */
/* a(n) = T(r,k)  where r(r+1)/2 ≤ n < (r+1)(r+2)/2  and  k = n - r(r+1)/2. */

#include <stdlib.h>
#include <stdbool.h>

/* --- static cache for the triangular array -------------------------------- */
static seq_t **T = NULL;            /* array of pointers to each row      */
static idx_t max_row = 0;           /* last computed row index             */
static bool cache_inited = false;   /* whether cache has been allocated    */

/* Ensure that rows 0..r are computed and cached. */
static void ensure_rows(idx_t r)
{
    if (cache_inited && r <= max_row)
        return;                     /* already computed                       */

    /* first use: initialise row 0 */
    if (!cache_inited) {
        T = (seq_t**)malloc(sizeof(seq_t*));
        if (!T) abort();
        T[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!T[0]) abort();
        T[0][0] = (seq_t)2;         /* T(0,0)=2 */
        max_row = 0;
        cache_inited = true;
    }

    /* extend the outer array if necessary */
    if (r > max_row) {
        seq_t **newT = (seq_t**)realloc(T, (size_t)(r + 1) * sizeof(seq_t*));
        if (!newT) abort();
        T = newT;

        for (idx_t i = max_row + 1; i <= r; ++i) {
            T[i] = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (!T[i]) abort();

            /* compute T(i,0) */
            if (i == 1) {
                T[i][0] = (seq_t)1; /* T(1,0)=1 */
            } else if (i == 0) {
                T[i][0] = (seq_t)2; /* already set for i==0 */
            } else {
                T[i][0] = T[i-1][0] + T[i-2][0];
            }

            /* compute the rest of the row */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t s1 = T[i-1][k-1];                     /* always valid */
                seq_t s2 = (k <= i-1) ? T[i-1][k] : (seq_t)0;

                seq_t s3;
                if (i >= 2 && k <= i-2)
                    s3 = T[i-2][k];
                else
                    s3 = (seq_t)0;

                T[i][k] = s1 + s2 + s3;
            }
        }
        max_row = r;
    }
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (true) {
        unsigned __int128 next = ((unsigned __int128)(r + 1) * (r + 2)) / 2;
        if (next > n)
            break;
        ++r;
    }

    /* position within the row */
    unsigned __int128 prev128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - (idx_t)prev128;   /* n and prev128 fit into idx_t for our range */

    ensure_rows(r);                    /* make sure the requested row exists */
    return T[r][k];
}
