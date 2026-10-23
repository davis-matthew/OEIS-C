/*
 * A132720  Sequence is identical to its second differences in absolute
 *          values.
 * https://oeis.org/A132720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; for n >= 1, a(n) = 2^⌊(2*n+3)/3⌋
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 46, u64 n <= 94, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute floor((2*n+3)/3) without overflow.
       Decompose n = 3*q + r, r in {0,1,2}.             */
    idx_t q = n / 3;
    idx_t r = n % 3;                       /* r is 0,1, or 2 */
    idx_t exp = q * 2 + (r * 2 + 3) / 3;   /* exponent = floor((2*n+3)/3) */

    seq_t result = 1;
    while (exp--) {
        result *= (seq_t)2;
    }
    return result;
}
