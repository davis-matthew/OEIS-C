/*
 * A088547  Primes of the form x^3+x^2+x+2.
 * https://oeis.org/A088547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^3 + x^2 + x + 2 */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index. Return the n‑th prime of the form
       x^3 + x^2 + x + 2. The search scans x from 0 upwards,
       tests the value with isPrimeGeneric, and counts primes. */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    seq_t val;
    idx_t x = 0;
    idx_t count = 0;

    for (;; ++x)
    {
        seq_t xx = (seq_t)x * (seq_t)x;                     /* x^2   */
        val = xx * (seq_t)x + xx + (seq_t)x + (seq_t)2;     /* x^3 + x^2 + x + 2 */

        if (isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
