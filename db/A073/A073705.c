/*
 * A073705  a(n) = Sum_{ d divides n } (n/d)^(2d).
 * https://oeis.org/A073705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{ d | n } (n/d)^(2d)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 62, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by enumerating divisors of n and computing the
 * required powers with binary exponentiation.  The implementation
 * uses only integer operations and the sequence arithmetic
 * operators provided by the project's build.  It handles all n ≥ 1
 * in any order of calls. */

#include <stdint.h>

/* Local fast exponentiation for seq_t values.
 * Uses binary exponentiation to minimize multiplications. */
static seq_t pow_seq_t_local(seq_t base, idx_t exp)
{
    seq_t result = 1;            /* seq_t literal is convertible */
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t total = 0;
    /* Enumerate all divisors d of n with 1 <= d <= sqrt(n) */
    for (idx_t d = 1; (unsigned __int128)d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t q = n / d;                /* complementary divisor */
            /* term for divisor d: (n/d)^(2d) = q^(2d) */
            total += pow_seq_t_local((seq_t)q, 2 * d);
            if (d != q)
            {
                /* term for divisor q: (n/q)^(2q) = d^(2q) */
                total += pow_seq_t_local((seq_t)d, 2 * q);
            }
        }
    }
    return total;
}
