/*
 * A084672  Expansion of g.f.: (1+x^2+2*x^4)/((1-x^3)*(1-x)^2).
 * https://oeis.org/A084672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(2*n^2 - n + 3 + 2*floor((n+2)/3) + floor((n+1)/3))
 * Range:     u8 n <= 11, u16 n <= 268, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor((n+2)/3) and floor((n+1)/3) without overflow */
    idx_t m = n / 3;
    idx_t r = n % 3;
    idx_t t1 = m + ((r + 2) / 3);   /* floor((n+2)/3) */
    idx_t t2 = m + ((r + 1) / 3);   /* floor((n+1)/3) */

    seq_t nn   = (seq_t)n;
    seq_t term = ((nn * nn) * (seq_t)2) - nn;   /* 2*n^2 - n */

    /* numerator = 2*n^2 - n + 3 + 2*floor((n+2)/3) + floor((n+1)/3) */
    term = term + (seq_t)3 + (seq_t)(2 * t1) + (seq_t)t2;

    return term / (seq_t)3;
}
