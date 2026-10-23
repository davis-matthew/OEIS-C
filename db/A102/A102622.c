/*
 * A102622  Nonprime numbers n concatenated n times.
 * https://oeis.org/A102622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the n‑th nonprime number repeated that
 *            many times
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime, num_digits, pow10_idx, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Primality test for 64‑bit numbers */
#include "../utils/is_prime.h"

/* Count decimal digits of an unsigned 64‑bit integer */
#include "../utils/num_digits.h"

/* Power of 10 as a seq_t */
#include "../utils/pow10_idx.h"

/* Big integer exponentiation */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset   */

    idx_t count = 0;                         /* how many nonprimes seen */
    idx_t cand  = 1;                          /* current candidate number */
    idx_t m     = 0;                          /* nth nonprime */

    /* Find the n‑th nonprime number */
    while (count < n)
    {
        if (!is_prime((uint64_t)cand))
        {
            ++count;
            if (count == n)
                m = cand;
        }
        ++cand;
    }

    /* Number of decimal digits of m */
    int d = num_digits((uint64_t)m);
    seq_t mult = pow10_idx((idx_t)d);         /* 10^d */

    /* Concatenate m, m times */
    seq_t result = 0;
    seq_t m_val  = m;                         /* convert to seq_t */
    for (idx_t i = 0; i < m; ++i)
    {
        result = result * mult + m_val;
    }

    return result;
}
