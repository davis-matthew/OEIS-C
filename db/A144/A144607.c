/*
 * A144607  Christoffel word of slope 9/11.
 * https://oeis.org/A144607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if (( (n+1)*9 mod 20 ) < 9) else 0 This implements the
 *            binary Christoffel word of slope 9/11 (lower mechanical word of
 *            slope 9/11) with period 20
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 9;          /* numerator of slope */
    const idx_t Q = 11;         /* denominator of slope */
    const idx_t N = P + Q;      /* period length = 20 */

    /* shift by one to match the sequence offset */
    idx_t i   = (n + 1) % N;
    idx_t prod = (i * P) % N;   /* (n+1)*P modulo N */

    return (prod < P) ? (seq_t)1 : (seq_t)0;
}
