/*
 * A191628  a(n) = floor((3^n)/(3*n - 2)).
 * https://oeis.org/A191628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor((3^n)/(3*n - 2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Exact denominator: 3*n - 2 */
    seq_t denom = (seq_t)3 * n - 2;

    /* If seq_t fits in 8 bytes or less, use 128‑bit intermediate
       to avoid overflow of the intermediate power. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 num = 1;
        for (idx_t i = 0; i < n; ++i) {
            num *= 3;
        }
        return (seq_t)(num / denom);
    }
    /* For larger seq_t types (big‑integer builds) the multiplication
       automatically produces arbitrary‑precision values. */
    else {
        seq_t num = 1;
        for (idx_t i = 0; i < n; ++i) {
            num *= 3;
        }
        return num / denom;
    }
}
