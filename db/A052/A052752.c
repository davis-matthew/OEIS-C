/*
 * A052752  a(n) = (3*n+1)^(n-1).
 * https://oeis.org/A052752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 0 */
    if (n == 0) return (seq_t)1;

    /* Compute base 3*n + 1 as seq_t */
    seq_t base = ((seq_t)3 * (seq_t)n) + (seq_t)1;

    /* Exponent n-1 (safe because n>=1) */
    idx_t exp = n - 1;

    /* Binary exponentiation */
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}
