/*
 * A130758  a(n) = n if n is not an odd prime number. Otherwise, a(n) = k,
 *          where k is the smallest integer such that n < 10^k.
 * https://oeis.org/A130758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 if (n-1) is not an odd prime; otherwise a(n) = number
 *            of decimal digits of n-1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_len, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    idx_t m_idx = n - 1;
    seq_t m = (seq_t)m_idx;
    if (m_idx > 2 && (m_idx & 1U) && is_prime(m_idx)) {
        return (seq_t)digit_len(m);
    }
    return m;
}
