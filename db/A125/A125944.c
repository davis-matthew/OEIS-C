/*
 * A125944  Sprague-Grundy values for octal game 4.726.
 * https://oeis.org/A125944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic Sprague–Grundy values for octal game 4.726.
   a(n) = {1,2,4,6} for n = 1..4,
   thereafter a(n) = period[(n-5) mod 16] with
   period = {1,2,7,4,1,8,2,4,1,8,7,11,1,2,4,8}. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Period after n = 4 */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)4,
        (seq_t)1, (seq_t)8, (seq_t)2, (seq_t)4,
        (seq_t)1, (seq_t)8, (seq_t)7, (seq_t)11,
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8
    };

    if (n <= 4) {
        switch ((idx_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)4;
            case 4: return (seq_t)6;
        }
    }

    /* n >= 5 */
    size_t idx = (size_t)((n - 5) % 16);
    return period[idx];
}
