/*
 * A096315  Dimensions n such that the integer lattice Z^n contains n+1
 *          equidistant points (i.e., the vertices of a regular n-simplex).
 * https://oeis.org/A096315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth positive integer m such that * ((m % 4) == 3) OR *
 *            ((m % 2) == 0 AND (m + 1) is a perfect square) OR * ((m % 4) ==
 *            1 AND (m + 1) is a sum of two squares)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Determine whether integer x can be expressed as the sum of two squares.
 * Uses the theorem that every prime ≡ 3 (mod 4) must occur with an even
 * exponent in the prime factorisation of x. */
static bool is_sum_of_two_squares(idx_t x)
{
    idx_t tmp = x;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            if ((p % 4) == 3 && (exp & 1))
                return false;
        }
    }
    if (tmp > 1 && (tmp % 4) == 3)
        return false;
    return true;
}

/* Test whether a small 64‑bit value is a perfect square. */
static bool is_square_seq(seq_t x)
{
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r * r == x;
}

/* Return the n-th term of sequence A096315. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t m = 1; ; ++m)
    {
        if ((m % 4) == 3)                /* case 2 */
        {
            ++count;
            if (count == n)
                return m;
            continue;
        }

        if ((m % 2) == 0)                /* case 1 */
        {
            seq_t mp1 = m + 1;
            if (is_square_seq(mp1))
            {
                ++count;
                if (count == n)
                    return m;
                continue;
            }
        }

        if ((m % 4) == 1)                /* case 3 */
        {
            if (is_sum_of_two_squares((idx_t)(m + 1)))
            {
                ++count;
                if (count == n)
                    return m;
            }
        }
    }
}
