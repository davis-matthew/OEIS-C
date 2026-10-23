/*
 * A140820  Triangle read by rows: T(n,k) = 1 if and only if the Gray codes
 *          for n and k have no bits in common.
 * https://oeis.org/A140820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   not stated
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A140820: a(n) is the row‐by‐row reading of the triangle
 * T(r,k) with T(r,k)=1 iff (Gray(r) & Gray(k)) == 0.
 * a(n) = 1, if the Gray code of row r and column k have no bits in common;
 * a(n) = 0 otherwise.  r and k are determined by the position of n
 * in the concatenated rows of the triangle. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row `r` such that the first n elements are exactly the
     * rows before `r`, i.e.  r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t sum = 0, r = 0;
    while (1) {
        /* Remaining elements to reach n from current sum */
        idx_t remaining = n - sum;
        if (remaining < r + 1)          /* next row would overflow `n` */
            break;
        sum += r + 1;      /* elements added by row r */
        r++;               /* move to next row */
    }
    /* Column index within the row */
    idx_t k = n - sum;

    /* Gray codes */
    idx_t gray_r = r ^ (r >> 1);
    idx_t gray_k = k ^ (k >> 1);

    /* Return 1 if no common bits, else 0. */
    return ((seq_t)((gray_r & gray_k) == 0));
}
