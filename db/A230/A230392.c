/*
 * A230392  Primes of the form 232*m^2+1.
 * https://oeis.org/A230392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 232*m^2+1 (OEIS A230392)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 12, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for offset 0 */

    idx_t    count   = 0;
    idx_t    m       = 1;
    seq_t    candidate;

    while (count < n) {
        seq_t m_sq   = (seq_t)m * (seq_t)m;   /* m^2 */
        candidate    = m_sq * (seq_t)232 + (seq_t)1;

        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++m;
    }

    return (seq_t)0; /* unreachable */
}
