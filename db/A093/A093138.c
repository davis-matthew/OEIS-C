/*
 * A093138  Expansion of (1-7x)/(1-10x).
 * https://oeis.org/A093138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, else a(n) = 3 * 10^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)1;

    /* fast exponentiation of 10^(n-1) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)10;
    idx_t exp    = n - 1;            /* n-1 is safe because n != 0 */

    while (exp != 0) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* multiply by 3 to obtain the final term */
    result *= (seq_t)3;
    return result;
}
