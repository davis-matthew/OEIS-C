/*
 * A082210  Square root of the squares arising in A082209.
 * https://oeis.org/A082210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n ≥ 2, a(n) repeats the 6‑term cycle [4, 8, 7, 31,
 *            248, 71]. This follows from a(n) = √( concat( A082209(n),
 *            A082209(n+1) ) ) with the periodic sequence A082209 cyclical as
 *            [1, 6, 4, 9, 61, 504]
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t period[6] = {
    (seq_t)4,
    (seq_t)8,
    (seq_t)7,
    (seq_t)31,
    (seq_t)248,
    (seq_t)71
};

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    /* n ≥ 2: compute position in the 6‑cycle */
    idx_t pos = (n - 2) % 6;
    return period[pos];
}
