/*
 * A201306  Primes of the form 10n^3+9.
 * https://oeis.org/A201306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 10·m³+9, where m runs over the
 *            positive integers
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    seq_t p = 0;
    while (count < n) {
        seq_t m_seq = (seq_t)m;
        p = ((seq_t)10 * m_seq * m_seq * m_seq) + 9;
        if (is_prime_seq(p)) {
            if (++count == n) return p;
        }
        ++m;
    }
    return 0; /* should never happen for n ≥ 1 */
}
