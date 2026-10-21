/*
 * A052652  E.g.f. x^4/(1-2x).
 * https://oeis.org/A052652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for 0 ≤ n < 4 * a(n) = 2^(n-4) * n! for n ≥ 4 *
 *            (derived from the recurrence a(n+1) = (2n+2) * a(n) with
 *            a(4)=24)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    seq_t result = 1;

    /* multiply by 2^(n-4) */
    for (idx_t i = 0; i < n - 4; ++i)
        result <<= 1;

    /* multiply by factorial n! */
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;

    return result;
}
