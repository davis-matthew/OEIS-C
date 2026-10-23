/*
 * A018414  Divisors of 364.
 * https://oeis.org/A018414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 364 in increasing order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 364 = 2^2 * 7 * 13 */
    const seq_t pow2[3] = { (seq_t)1, (seq_t)2, (seq_t)4 };
    const seq_t pow7[2] = { (seq_t)1, (seq_t)7 };
    const seq_t pow13[2] = { (seq_t)1, (seq_t)13 };

    /* there are 3*2*2 = 12 divisors */
    seq_t divs[12];
    size_t idx = 0;

    for (size_t e2 = 0; e2 < 3; ++e2) {
        for (size_t e7 = 0; e7 < 2; ++e7) {
            for (size_t e13 = 0; e13 < 2; ++e13) {
                divs[idx++] = pow2[e2] * pow7[e7] * pow13[e13];
            }
        }
    }

    /* simple insertion sort (12 elements, trivial overhead) */
    for (size_t i = 1; i < 12; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j-1] > key) {
            divs[j] = divs[j-1];
            --j;
        }
        divs[j] = key;
    }

    if (n == 0 || n > 12) return (seq_t)0;
    return divs[n-1];
}
