/*
 * A117904  Number triangle [k<=n]*0^abs(L(C(n,2)/3) - L(C(k,2)/3)) where
 *          L(j/p) is the Legendre symbol of j and p.
 * https://oeis.org/A117904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   f_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/f_mod3.h"
/*  A117904 – Number triangle:     T(n,k)=1 if L(C(n,2)/3)=L(C(k,2)/3) else 0
    The sequence returned is the flattened triangle (row‑major order):
      a(n) = T(row, col)   where
        row = floor((sqrt(8*n+1)-1)/2)
        col = n - row(row+1)/2
    Legendre symbol modulo 3:  L(j/3)=1 if j≡±1 (mod 3) otherwise 0.
    Since C(n,2)=n(n‑1)/2, L(C(n,2)/3)=0 for n≡0,1,3,4 (mod 6) and =1 for n≡2,5 (mod 6).  */

/* helper: largest r with r(r+1)/2 <= n  (triangular root) */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;          /* ceil((low+high)/2) */
        __int128 tri = (__int128)mid * (mid + 1) / 2;    /* mid*(mid+1)/2 */
        if (tri <= (__int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* find the row containing the n-th element (row indices start at 0) */
    idx_t row = triangular_root(n);

    /* offset of the first element in that row */
    __int128 prefix = (__int128)row * (row + 1) / 2;     /* row(row+1)/2 */
    idx_t col = n - (idx_t)prefix;                       /* column within the row */

    /* compute Legendre symbols for row and column */
    seq_t f_row = f_mod3(row);
    seq_t f_col = f_mod3(col);

    /* T(row,col) equals 1 iff the symbols are equal */
    return (f_row == f_col) ? (seq_t)1 : (seq_t)0;
}
