/*
 * A196153  Primes of the form 2*n^3 + 5*n^2 + 3*n + 1.
 * https://oeis.org/A196153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 14, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196153: primes of the form 2*n^3 + 5*n^2 + 3*n + 1,
   enumerated by the index of the prime value. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t i = 1;
    idx_t count = 0;
    while (1)
    {
        seq_t ii   = (seq_t)i;
        seq_t n2   = ii * ii;          /* n^2   */
        seq_t n3   = n2 * ii;          /* n^3   */
        seq_t val  = n3 * 2 + n2 * 5 + ii * 3 + 1;  /* 2n^3+5n^2+3n+1 */
        if (is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++i;
    }
}
