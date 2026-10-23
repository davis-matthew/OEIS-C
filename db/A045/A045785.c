/*
 * A045785  Squares with initial digit '2'.
 * https://oeis.org/A045785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045785 - Squares with initial digit '2'
   a(n) = (m_n)^2, where m_n is the n-th integer whose square starts
   with the decimal digit '2'. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Assume n >= 1 (offset 1). No behavior is defined for n=0. */
    idx_t count = 0;
    seq_t m = 1;           /* Candidate integer whose square we test */
    while (1)
    {
        seq_t sq = m * m;  /* m^2 (safe: m stays small for n <= 10^4) */
        seq_t leading = sq;
        /* Extract the most significant decimal digit */
        while (leading >= 10)
            leading /= 10;
        if (leading == 2)
        {
            ++count;
            if (count == n)
                return sq;
        }
        ++m;
    }
}
