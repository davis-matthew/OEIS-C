/*
 * A118720  Cubes which are divisible by the sum of their digits.
 * https://oeis.org/A118720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th cube that is divisible by the sum of its
 *            decimal digits. * It is defined as the n-th term m^3 such that
 *            m^3 % sum_of_decimal_digits(m^3) == 0
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 308, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */

    idx_t found = 0;
    uint64_t m = 0;               /* m is the cube root, fits in uint64_t */

    while (found < n) {
        ++m;
        seq_t m_seq = (seq_t)m;
        seq_t cube   = m_seq * (m_seq * m_seq);   /* compute m^3 */
        seq_t sum    = digit_sum(cube);

        if (sum != 0 && cube % sum == 0) {
            ++found;
            if (found == n)
                return cube;
        }
    }
    return 0;  /* should never happen for n >= 1 */
}
