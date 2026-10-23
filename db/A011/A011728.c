/*
 * A011728  A binary m-sequence: expansion of reciprocal of x^15+x+1.
 * https://oeis.org/A011728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) XOR a(n-15), with a(0..13)=0, a(14)=1
 * Range:     u8 n <= 9890, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    const idx_t period = ((idx_t)1 << 15) - 1;      /* 2^15 - 1 = 32767 */
    static seq_t cache[32767];
    static bool initialized = false;

    if (!initialized) {
        /* initial values: 14 zeros then one 1 */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = (i == 14) ? (seq_t)1 : (seq_t)0;
        /* recurrence: a(n) = a(n-1) XOR a(n-15) */
        for (idx_t i = 15; i < period; ++i)
            cache[i] = cache[i - 1] ^ cache[i - 15];
        initialized = true;
    }

    idx_t m = n % period;
    return cache[m];
}
