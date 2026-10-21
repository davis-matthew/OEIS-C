/*
 * A052636  Expansion of e.g.f. (2-x-2*x^2)/((1-x)*(1-2*x^2)).
 * https://oeis.org/A052636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! if n is odd; * a(n) = n! * (2^(n/2) + 1) if n is even.
 *            * (This matches the OEIS recurrence a(n)=n!*A052552(n) with
 *            A052552(n)=1 for odd n and 2^(n/2)+1 for even n.)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    if ((n & 1) == 0) {          /* even n */
        idx_t exp = n >> 1;      /* n/2 */
        seq_t pow2 = 1;
        for (idx_t i = 0; i < exp; ++i)
            pow2 *= 2;
        seq_t factor = pow2 + 1;
        return fact * factor;
    }

    /* odd n */
    return fact;
}
