/*
 * A164984  Odd (Jacobsthal) triangle.
 * https://oeis.org/A164984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the Odd (Jacobsthal) triangle read
 *            row by row. The triangle T(n,k) (1 ≤ k ≤ n) satisfies T(1,1)=1,
 *            T(2,1)=T(2,2)=1, T(n,k) =
 *            T(n-1,k)+T(n-1,k-1)+2*T(n-2,k)+T(n-2,k-1) for n≥3
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*-----------------  helper to compute floor(sqrt(x)) for uint64_t  --------*/
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* highest power of 4 ≤ x   */
    while (bit > 0)
    {
        if (r + bit <= x)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        bit >>= 2;
    }
    return r;
}

/*-----------------  find the row r such that  r*(r+1)/2 ≥ n  -------------*/
static inline idx_t row_of_n(idx_t n)
{
    idx_t disc   = 8*n + 1;                 /* 8*n+1 is always <= 2^64-1 for n up to 2^63-1 */
    idx_t root   = isqrt_u64(disc);
    idx_t r      = (root - 1) / 2;           /* lower bound of the answer   */
    if (r*(r+1)/2 < n) ++r;                 /* correct for the floor case   */
    return r;
}

/*-----------------  storage of the triangle  ----------------------------*/
static seq_t *tri = NULL;
static idx_t  max_row = 0;

/*-----------------  ensure rows 1..r are computed  -----------------------*/
static void ensure_rows_up_to(idx_t r)
{
    if (r <= max_row) return;

    /* new required size */
    idx_t new_size = r*(r+1)/2;
    seq_t *new_tri = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_tri) return;   /* out of memory – caller will get garbage on failure */

    /* copy old data if any */
    if (tri)
    {
        idx_t old_size = max_row*(max_row+1)/2;
        for (idx_t i = 0; i < old_size; ++i) new_tri[i] = tri[i];
        free(tri);
    }
    tri = new_tri;

    /* initialise first row if necessary */
    if (max_row == 0)
    {
        tri[0] = 1;
        max_row = 1;
    }

    /* compute new rows */
    for (idx_t i = max_row + 1; i <= r; ++i)
    {
        idx_t offset_i = (i-1)*i/2;           /* zero based start of row i          */

        if (i == 2)                         /* base cases                     */
        {
            tri[offset_i]     = 1;           /* T(2,1) */
            tri[offset_i + 1] = 1;           /* T(2,2) */
        }
        else
        {
            idx_t offset_prev1 = (i-2)*(i-1)/2;   /* row n-1                 */
            idx_t offset_prev2 = (i-3)*(i-2)/2;   /* row n-2                 */

            for (idx_t j = 1; j <= i; ++j)      /* 1 ≤ j ≤ i (column)     */
            {
                seq_t val = 0;

                /* T(n-1,k)  :  k=j  -->  j ≤ i-1  */
                if (j <= i-1)
                    val += tri[offset_prev1 + (j-1)];
                /* T(n-1,k-1):  k-1=j-1  -->  j≥2          */
                if (j >= 2)
                    val += tri[offset_prev1 + (j-2)];
                /* 2*T(n-2,k) :  k=j  -->  j ≤ i-2 */
                if (j <= i-2)
                    val += tri[offset_prev2 + (j-1)] * 2;
                /* T(n-2,k-1):  k-1=j-1  -->  j≥2 && j≤i-1 */
                if (j >= 2 && j <= i-1)
                    val += tri[offset_prev2 + (j-2)];

                tri[offset_i + (j-1)] = val;
            }
        }
    }
    max_row = r;
}

/*-----------------  public function a(n)  --------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* safety: offset is 1 */

    idx_t r   = row_of_n(n);
    idx_t start = ((r-1)*r)/2 + 1;            /* 1‑based start of row r  */
    idx_t k    = n - start + 1;               /* column in the row      */

    ensure_rows_up_to(r);

    idx_t idx = ((r-1)*r)/2 + (k-1);          /* zero‑based index in tri*/
    return tri[idx];
}
