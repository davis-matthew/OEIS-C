/*
 * A178580  Expansion of the polynomial
 *          (1+x^3)*(1+x^9)*(1+x^11)*(1+x^15)*(1+x^17)*(1+x^23).
 * https://oeis.org/A178580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of subsets of {3,9,11,15,17,23} whose sum equals n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static seq_t counts[79] = {0};
    static int init = 0;
    if (!init) {
        static const idx_t ex[6] = {3, 9, 11, 15, 17, 23};
        for (idx_t mask = 0; mask < ((idx_t)1 << 6); ++mask) {
            idx_t sum = 0;
            for (int i = 0; i < 6; ++i)
                if (mask & ((idx_t)1 << i))
                    sum += ex[i];
            if (sum < 79)
                counts[(size_t)sum] += (seq_t)1;
        }
        init = 1;
    }
    return (n < 79) ? counts[(size_t)n] : (seq_t)0;
}
