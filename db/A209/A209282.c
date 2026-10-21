/*
 * A209282  a(n) is difference of indices of terms = 9 in A187824.
 * https://oeis.org/A209282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is periodic with period 16:
   a(n) = 161,288,1,70,90,470,89,1,70,1,89,470,90,70,1,288 repeated. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[16] = {
        (seq_t)161, (seq_t)288, (seq_t)1,   (seq_t)70,
        (seq_t)90,  (seq_t)470, (seq_t)89,  (seq_t)1,
        (seq_t)70,  (seq_t)1,   (seq_t)89,  (seq_t)470,
        (seq_t)90,  (seq_t)70,  (seq_t)1,   (seq_t)288
    };
    size_t idx = (size_t)((n - 1) % 16);
    return pattern[idx];
}
