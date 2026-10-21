/*
 * A083082  First subdiagonal of number array A083075.
 * https://oeis.org/A083082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)*(2n+5)^n + 1)/(n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the generic integer type. */
    seq_t nn = (seq_t)n;

    /* a1 = n + 1 */
    seq_t a1 = nn + 1;

    /* base = 2n + 5 */
    seq_t base = nn * 2 + 5;

    /* Compute base^n using binary exponentiation. */
    seq_t pow_b = 1;
    seq_t cur = base;
    idx_t exp = n;
    while (exp != 0) {
        if (exp & 1)
            pow_b = pow_b * cur;
        cur = cur * cur;
        exp >>= 1;
    }

    /* numerator = a1 * pow_b + 1 */
    seq_t num = a1 * pow_b + 1;

    /* denominator = n + 2 */
    seq_t den = nn + 2;

    /* Final division (exact by definition). */
    return num / den;
}
