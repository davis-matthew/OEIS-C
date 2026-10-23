/*
 * A123999  Number of ordered ways of writing n as a sum of 4 squares of
 *          nonnegative numbers less than 4.
 * https://oeis.org/A123999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Card{(a,b,c,d) | 0 <= a,b,c,d < 4 and a²+b²+c²+d² = n}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the counts for all n ≤ 36 (4·3²). */
    static seq_t cache[37];
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        for (idx_t i = 0; i < 37; ++i)
            cache[i] = 0;

        for (int a = 0; a < 4; ++a) {
            int a2 = a * a;
            for (int b = 0; b < 4; ++b) {
                int ab2 = a2 + b * b;
                for (int c = 0; c < 4; ++c) {
                    int abc2 = ab2 + c * c;
                    for (int d = 0; d < 4; ++d) {
                        int total = abc2 + d * d;
                        if (total < 37)
                            ++cache[total];
                    }
                }
            }
        }
    }

    if (n < 37)
        return cache[n];
    else
        return 0;
}
