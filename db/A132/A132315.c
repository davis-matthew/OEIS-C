/*
 * A132315  Sum of the non-fourth powers less than or equal to n.
 * https://oeis.org/A132315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - sum_{k=1}^{floor(n^{1/4})} k^4
 * Range:     u8 none, u16 none, u32 none, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for n < 1 */
    
    /* sum of all integers 1..n */
    seq_t nseq   = (seq_t)n;
    seq_t total  = nseq * (nseq + 1) / 2;

    /* compute r = floor( n^{1/4} )  (r <= 65535 for 64‑bit n) */
    idx_t low = 0, high = 65535;
    while (low < high) {
        idx_t mid  = (low + high + 1) >> 1;
        seq_t m4   = (seq_t)mid * mid * mid * mid;
        if (m4 <= n) low = mid;
        else          high = mid - 1;
    }
    idx_t r = low;

    /* sum of fourth powers 1^4 + 2^4 + ... + r^4  */
    seq_t sum4 = 0;
    if (r > 0) {
        seq_t R      = (seq_t)r;
        seq_t temp   = R * (R + 1);              /* R*(R+1) */
        temp        *= (2 * R + 1);              /* * (2R+1) */
        seq_t outer  = (3 * R * R + 3 * R - 1);  /* (3R^2 + 3R - 1) */
        temp        *= outer;                    /* product */
        sum4        = temp / 30;                  /* exact division */
    }

    return total - sum4;
}
