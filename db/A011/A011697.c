/*
 * A011697  A binary m-sequence: expansion of reciprocal of x^8+x^4+x^3+x^2+1.
 * https://oeis.org/A011697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) ⊕ a(n-3) ⊕ a(n-4) ⊕ a(n-8) for n ≥ 8 with initial
 *            values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Period is 255; precompute one cycle and return a[n mod 255]. */
    static bool init_flag = false;
    static seq_t table[255];

    if (!init_flag) {
        /* initialise first 8 terms */
        for (idx_t i = 0; i < 8; ++i)
            table[i] = (seq_t)0;
        table[7] = (seq_t)1;

        /* generate remaining terms of the period */
        for (idx_t i = 8; i < 255; ++i) {
            table[i] = table[i - 2] ^ table[i - 3] ^ table[i - 4] ^ table[i - 8];
        }
        init_flag = true;
    }

    return table[n % 255];
}
