/*
 * A066506  Least k > n^2 such that (k^3+1)/(n^2+1) is an integer.
 * https://oeis.org/A066506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n^2 such that (k^3+1)/(n^2+1) is an integer
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = n^2 + 1  -- divisor of k^3+1   */
    seq_t m = (seq_t)n * n + 1;
    /* start searching from k = n^2 + 1  */
    seq_t k = m;
    for (;;) {
        /* compute k^3 modulo m without overflow */
        seq_t km = k % m;
        seq_t t = km * km;        /* km^2          */
        t %= m;
        t = (t * km + 1) % m;     /* km^3 + 1 mod m */
        if (t == 0)
            return k;            /* k is the required value */
        ++k;
    }
}
