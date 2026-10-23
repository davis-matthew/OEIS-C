/*
 * A018662  Divisors of 816.
 * https://oeis.org/A018662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 816
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sequence has exactly 20 divisors. For n > 20 we return 0. */
    if (n == 0 || n > 20) {
        return (seq_t)0;
    }

    /* Generate all divisors: 2^a * 3^b * 17^c with 0≤a≤4, 0≤b≤1, 0≤c≤1. */
    seq_t divisors[20];
    size_t count = 0;
    for (idx_t a = 0; a <= 4; ++a) {
        seq_t va = (seq_t)1;
        for (idx_t i = 0; i < a; ++i) {
            va *= (seq_t)2;
        }
        for (idx_t b = 0; b <= 1; ++b) {
            seq_t vab = va;
            if (b) {
                vab *= (seq_t)3;
            }
            for (idx_t c = 0; c <= 1; ++c) {
                seq_t v = vab;
                if (c) {
                    v *= (seq_t)17;
                }
                divisors[count++] = v;
            }
        }
    }

    /* Simple insertion sort (20 elements, trivial cost). */
    for (size_t i = 1; i < count; ++i) {
        size_t j = i;
        while (j > 0 && divisors[j - 1] > divisors[j]) {
            seq_t tmp = divisors[j - 1];
            divisors[j - 1] = divisors[j];
            divisors[j] = tmp;
            --j;
        }
    }

    return divisors[n - 1];
}
