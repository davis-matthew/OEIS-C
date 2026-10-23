/*
 * A118032  Triangle T, read by rows, such that diagonal 2*n of T equals
 *          diagonal n of T^2 and diagonal 2*n+1 of T equals diagonal n of
 *          T*U: [T^2](n,k) = T(2*n-k,k) and [T*U](n,k) = T(2*n+1-k,k) for
 *          n>=k, k>=0, where U = SHIFT_UP(T).
 * https://oeis.org/A118032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 214, u32 n <= 1044, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A118032  – Triangle T read by rows
 *
 *  T(n,k) is defined for 0 ≤ k ≤ n by:
 *      T(n,n)   = 1
 *      T(n+1,n) = n+1
 *      For n > k+1, let m = n:
 *          If (m-k) is even, let p = (m+k)/2
 *              T(m,k) = Σ_{j=k}^{p} T(p,j) * T(j,k)
 *          If (m-k) is odd, let p = (m+k-1)/2
 *              T(m,k) = Σ_{j=k}^{p} T(p,j) * T(j+1,k)
 *
 *  a(n) is the sequence obtained by reading all T(n,k) by rows,
 *  i.e. a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ...
 */

#include <stdlib.h>

static inline idx_t row_start(idx_t r)
{
    return r * (r + 1) / 2;
}

static seq_t *tri      = NULL;     /* flattened triangle */
static idx_t   rows_filled = 0;    /* number of rows already computed */

static void ensure_rows(idx_t r)
{
    if (r < rows_filled)
        return; /* already have rows 0..r */

    /* Allocate space for rows up to r */
    idx_t new_len = (r + 1) * (r + 2) / 2;
    seq_t *new_tri = (seq_t *)realloc(tri, new_len * sizeof(seq_t));
    if (!new_tri)
        _Exit(1);
    tri = new_tri;

    /* Build missing rows */
    for (idx_t i = rows_filled; i <= r; ++i)
    {
        idx_t i_start = row_start(i);
        for (idx_t k = 0; k <= i; ++k)
        {
            idx_t pos = i_start + k;
            if (k == i) /* diagonal */
            {
                tri[pos] = (seq_t)1;
            }
            else if (k == i - 1) /* sub‑diagonal */
            {
                tri[pos] = (seq_t)(i);
            }
            else
            {
                seq_t sum = (seq_t)0;
                if ( ((i - k) & 1) == 0 ) /* even parity: use T^2 */
                {
                    idx_t p = (i + k) >> 1;          /* (i+k)/2 */
                    idx_t p_start = row_start(p);
                    for (idx_t j = k; j <= p; ++j)
                    {
                        seq_t a = tri[p_start + j];          /* T(p,j) */
                        seq_t b = tri[row_start(j) + k];      /* T(j,k) */
                        sum += a * b;
                    }
                }
                else /* odd parity: use T*U */
                {
                    idx_t p = (i + k - 1) >> 1;      /* (i+k-1)/2 */
                    idx_t p_start = row_start(p);
                    for (idx_t j = k; j <= p; ++j)
                    {
                        seq_t a = tri[p_start + j];              /* T(p,j) */
                        seq_t b = tri[row_start(j + 1) + k];      /* T(j+1,k) */
                        sum += a * b;
                    }
                }
                tri[pos] = sum;
            }
        }
    }

    rows_filled = r + 1;
}

seq_t a(idx_t n)
{
    /* Find the row containing index n */
    idx_t r = 0;
    while (row_start(r + 1) <= n)
        ++r;

    ensure_rows(r);

    idx_t pos = row_start(r) + (n - row_start(r));
    return tri[pos];
}
