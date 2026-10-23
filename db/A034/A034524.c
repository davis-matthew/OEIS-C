/*
 * A034524  a(n) = 11^n + 1.
 * https://oeis.org/A034524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return (seq_t)2;   /* 11^0 + 1 */

    seq_t result = 1;
    seq_t base = (seq_t)11;
    idx_t exp = n;

    /* Exponentiation by squaring */
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    result += 1;   /* add the constant +1 */
    return result;
}
