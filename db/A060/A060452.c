/*
 * A060452  Let v = (1,4,9,...,n^2), x = (0,1,2,4,6,...) [first n terms of
 *          A002620]; a(n) = v.v * x.x - (v.x)^2.
 * https://oeis.org/A060452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ( Σ i⁴ ) · ( Σ ⌊i²/4⌋² ) – ( Σ i²·⌊i²/4⌋ )²
 * where the sums run from i = 1 to n.
 * If needed, ⌊i²/4⌋ = floor(i² / 4) is the nth term of OEIS A002620.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t S4   = 0;   /* Σ i⁴          */
    seq_t Sx2  = 0;   /* Σ ⌊i²/4⌋²     */
    seq_t Svx  = 0;   /* Σ i²·⌊i²/4⌋   */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii   = (seq_t)i * (seq_t)i;           /* i²           */
        seq_t iiii = ii * ii;                       /* i⁴           */
        S4 += iiii;

        seq_t x = (ii) / (seq_t)4;                  /* ⌊i²/4⌋       */
        seq_t xx = x * x;                           /* ⌊i²/4⌋²     */
        Sx2 += xx;
        Svx += ii * x;
    }

    seq_t ans = S4 * Sx2 - Svx * Svx;
    return ans;
}
