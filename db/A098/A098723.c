/*
 * A098723  a(n) = C(n, 4)^(n-5).
 * https://oeis.org/A098723
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,4)^(n-5)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the values are not defined by the sequence. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* The first two terms are 1 by definition. */
    if (n <= 5) {
        return (seq_t)1;
    }

    /* Compute C(n,4) = n*(n-1)*(n-2)*(n-3)/24. */
    seq_t base = ((seq_t)n * (n - 1) * (n - 2) * (n - 3)) / 24;

    /* Exponent is n-5. */
    idx_t exp = n - 5;

    /* Binary exponentiation: result *= base whenever bit is set. */
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {            /* avoid unnecessary multiplication when exp becomes 0 */
            base *= base;
        }
    }

    return result;
}
