/*
 * A130200  Evil triangular numbers.
 * https://oeis.org/A130200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th Evil Triangular number (triangular numbers whose
 *            binary representation contains an even number of 1s)
 * Range:     u8 n <= 16, u16 n <= 201, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_evil(seq_t x)
{
    uint8_t parity = 0;
    while (x) {
        parity ^= (uint8_t)(x & 1);
        x >>= 1;
    }
    return parity == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* For safety but per offset 1, n>=1 */

    idx_t k = 0;                   /* triangular index (starts at 0) */
    idx_t found = 0;

    for (;;) {
        seq_t t = ((seq_t)k * ((seq_t)k + 1)) / 2;
        if (is_evil(t)) {
            if (++found == n) return t;
        }
        ++k;
    }
}
