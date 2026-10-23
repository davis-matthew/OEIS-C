/*
 * A178189  Numerators of sum (1/5)^((k^2+3k)/2) from k=1 to n.
 * https://oeis.org/A178189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1) = 1 a(n) = a(n-1) * 5^(n+1) + 1 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;        /* a(1) */
    if (n <= 1)
        return result;

    /* pow5 holds 5^(i+1) during the loop.
       Start at 5^3 (used when i = 2). */
    seq_t pow5 = 125;        /* 5^3 */

    for (idx_t i = 2; i <= n; ++i) {
        result = result * pow5 + 1;
        if (i < n)            /* prepare exponent for next step */
            pow5 *= 5;
    }

    return result;
}
