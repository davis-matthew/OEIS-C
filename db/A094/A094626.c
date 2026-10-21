/*
 * A094626  Expansion of x*(1+x)/((1-x)*(1-10*x^2)).
 * https://oeis.org/A094626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 2*(10^(n/2)-1)/9 (n even) * a(n) = (11*10^((n-1)/2)-2)/9
 *            (n odd) * * The value can be built digit by digit: * • If n is
 *            even, the decimal representation consists of n/2 copies of the
 *            digit 2. * • If n is odd, the decimal representation consists of
 *            a leading 1 followed by * (n-1)/2 copies o...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* only for bool, no output */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Even indices: produce n/2 copies of digit 2 */
    if ((n & 1) == 0) {
        idx_t loops = n >> 1;          /* n/2 */
        seq_t val = 0;
        for (idx_t i = 0; i < loops; ++i)
            val = val * (seq_t)10 + (seq_t)2;
        return val;
    }

    /* Odd indices: start with 1 then add (n-1)/2 copies of digit 2 */
    idx_t loops = (n - 1) >> 1;        /* (n-1)/2 */
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < loops; ++i)
        val = val * (seq_t)10 + (seq_t)2;
    return val;
}
