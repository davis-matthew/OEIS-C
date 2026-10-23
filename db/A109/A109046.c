/*
 * A109046  a(n) = lcm(n, 5).
 * https://oeis.org/A109046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n, 5) (for n ≥ 0, a(0) = 0)
 * Range:     u8 n <= 51, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 5 is prime, so gcd(n,5) is either 1 or 5.
       Hence lcm(n,5) = n          if 5 | n
                    = 5 * n        otherwise. */
    if (n % 5 == 0)
        return (seq_t)n;
    else
        return (seq_t)n * (seq_t)5;
}
