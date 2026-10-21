/*
 * A064894  Binary dilution of n. GCD of exponents in binary expansion of n.
 * https://oeis.org/A064894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = GCD(e0, e1, …) where n = Σ 2^ei (binary exponents)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0 return 0. */

#include <stdint.h>

/* Local gcd for unsigned integers (fits in seq_t). */
static seq_t local_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* a(0) = 0 by definition */

    seq_t result = 0;
    int pos = 0;
    idx_t x = n;

    while (x != 0)
    {
        if (x & 1ULL)            /* bit at position 'pos' is set */
        {
            if (result == 0)      /* first set bit: initialise */
                result = pos;
            else
                result = local_gcd(result, pos);
        }
        x >>= 1;
        pos++;
    }
    return result;
}
