/*
 * A125557  Primes in the sequence a(n)=n^2+3/2-1/2*(-1)^n.
 * https://oeis.org/A125557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + 1 when n is even, n^2 + 2 when n is odd, but only
 *            the * primes of that form are included in the sequence. Since
 *            not every such * value is prime, the algorithm searches
 *            sequentially for the nth prime of * the form n^2 + 1 (even) or
 *            n^2 + 2 (odd)
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        seq_t val = (seq_t)m * (seq_t)m;
        val += (m & 1ULL) ? 2 : 1;   /* add 2 for odd m, 1 for even m */
        if (isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
