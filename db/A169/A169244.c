/*
 * A169244  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39 * 38^(n-1) for n >= 1, a(0) = 1 * (This follows from
 *            the generating function (1+t)/(1-38t).)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 we return the first term directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 38^(n-1) using exponentiation by squaring. */
    idx_t exp = n - 1;
    seq_t result = (seq_t)1;
    seq_t power  = (seq_t)38;

    while (exp > 0) {
        if (exp & 1) {
            result *= power;      /* multiply by current power */
        }
        exp >>= 1;
        if (exp) {
            power *= power;        /* square the power for next bit */
        }
    }

    /* Multiply by the leading coefficient 39 to obtain a(n). */
    return (seq_t)39 * result;
}
