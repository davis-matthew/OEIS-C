/*
 * A233207  Triangle T(n,k), read by rows, given by T(n+k,k)=2*k*(2*n+1).
 * https://oeis.org/A233207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*k*(2*(r-k)+1) where r = floor((sqrt(1+8*n)-1)/2) k = n
 *            - r*(r+1)/2 (r,k) gives the row and column of the triangle
 *            T(r,k) with T(r,k) = 2*k*(2*(r-k)+1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Find the row r that contains index n.  Use the explicit formula
       and correct for any rounding errors from floating arithmetic.    */
    long double d = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Adjust to ensure r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2.   */
    unsigned __int128 pref = (unsigned __int128)r * (r + 1) / 2;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) {
        r++;
        pref = (unsigned __int128)r * (r + 1) / 2;
    }
    while (pref > n) {
        r--;
        pref = (unsigned __int128)r * (r + 1) / 2;
    }

    /* Column index k.                                      */
    idx_t k = n - (idx_t)pref;

    /* Compute the triangle entry using seq_t arithmetic.    */
    seq_t seq_r = (seq_t)r;
    seq_t seq_k = (seq_t)k;
    seq_t two   = (seq_t)2;

    seq_t a_val = two * seq_k;
    seq_t b_val = two * (seq_r - seq_k) + (seq_t)1;
    seq_t res   = a_val * b_val;

    return res;
}
