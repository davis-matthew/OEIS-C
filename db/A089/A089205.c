/*
 * A089205  a(n) = n^n * (n-1).
 * https://oeis.org/A089205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n * (n-1) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=1 the term is 0 by definition. */
    if (n <= 1)
        return (seq_t)0;

    /* fast exponentiation: compute n^n */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n;
    idx_t exp    = n;

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* multiply by (n-1) */
    result *= (seq_t)(n - 1);
    return result;
}
