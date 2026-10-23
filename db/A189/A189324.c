/*
 * A189324  Number of nondecreasing arrangements of n+2 numbers in 0..7 with
 *          the last equal to 7 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 11,17,31,51,91,157,253,374,509 for 1≤n≤9
   a(n) = 140*n - 751 for n≥10    */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)11, (seq_t)17, (seq_t)31,
        (seq_t)51, (seq_t)91, (seq_t)157,
        (seq_t)253,(seq_t)374,(seq_t)509
    };

    if (n <= (idx_t)9) {
        return init[(size_t)(n - 1)];
    }

    idx_t val = (idx_t)140 * n - 751;
    return (seq_t)val;
}
