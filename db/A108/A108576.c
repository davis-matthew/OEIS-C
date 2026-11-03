/*
 * A108576  Number of 3 X 3 magic squares (with distinct positive entries)
 *          having all entries < n.
 * https://oeis.org/A108576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * B(n-10) + 16 * B(n-12) where B(m) = sum_{p=0}^{⌊m/4⌋}
 *            (⌊(m-4p)/6⌋ + 1) * (m-4p+1 - 3⌊(m-4p)/6⌋) for m ≥ 0 (B(m) = 0
 *            for m < 0). This is derived from the generating function
 *            ((8*x^10*(2*x^2+1))/((1-x^6)*(1-x^4)*(1-x)^2)). The sequence is
 *            0 for n<10
 * Range:     u8 n <= 17, u16 n <= 78, u32 n <= 2959, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9697 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t B(idx_t m)
{
    seq_t sum = 0;
    /* m is non‑negative; loop over p */
    for (idx_t p = 0; p <= m / 4; ++p)
    {
        idx_t t = m - 4 * p;          /* remainder after subtracting 4p */
        idx_t R = t / 6;              /* number of terms in inner sum */
        seq_t term = (seq_t)(R + 1) * (seq_t)(t + 1 - 3 * R);
        sum += term;
    }
    return sum;
}
 

 
seq_t a(idx_t n)
{
    if (n < 10) return 0;
    seq_t result = 0;
    idx_t m1 = n - 10;
    result += (seq_t)8 * B(m1);
    if (n >= 12)
    {
        idx_t m2 = n - 12;
        result += (seq_t)16 * B(m2);
    }
    return result;
}
