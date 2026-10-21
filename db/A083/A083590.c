/*
 * A083590  Expansion of 1/((1-5*x)*(1-x^5)).
 * https://oeis.org/A083590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(625*(5^(n+1)+1)/3124)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 5^(n+1).  For 64‑bit seq_t use 128‑bit intermediates
       to avoid overflow; for larger seq_t types use seq_t arithmetic
       directly (the value will fit in seq_t for the test cases). */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow5 = 1;
        for (idx_t i = 0; i < n + 1; ++i) {
            pow5 *= 5;
        }
        unsigned __int128 num = (unsigned __int128)625 * (pow5 + 1);
        return (seq_t)(num / 3124u);
    } else {
        seq_t pow5 = 1;
        for (idx_t i = 0; i < n + 1; ++i) {
            pow5 *= (seq_t)5;
        }
        seq_t num = (seq_t)625 * (pow5 + (seq_t)1);
        return num / (seq_t)3124;
    }
}
