/*
 * A163283  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n^3 and ending with n^4, such that the difference between
 *          successive terms is equal to n^3 - n^2.
 * https://oeis.org/A163283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 151, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the linear index of OEIS sequence A163283.
   The sequence is given by the triangle
          T(n,k) = n³ + k·(n³ – n²)          (0 ≤ k ≤ n, n ≥ 0)
   and is read row‑wise:  a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /*---  determine the row r and column k for the linear index n  ---*/
    idx_t r = 0;
    for (;;) {
        /* use 128‑bit intermediate to avoid overflow when r is large */
        unsigned __int128 off = (unsigned __int128)r * (r + 1) / 2;
        if (off > n) {
            /* the desired index lies in previous row */
            r -= 1;
            break;
        }
        r += 1;
    }

    /* offset of the first element of row r (in 0‑based indexing) */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* column index within the row   */

    /*---  compute the value using seq_t arithmetic  ---*/
    seq_t r_seq = (seq_t)r;
    seq_t r2    = r_seq * r_seq;          /* r²  */
    seq_t r3    = r2 * r_seq;             /* r³  */
    seq_t diff  = r3 - r2;                /* r³ – r² */

    return r3 + (seq_t)k * diff;          /* T(r,k) */
}
