/*
 * A098721  a(n) = C(n, 2)^(n-3) = (n(n-1)/2)^(n-3).
 * https://oeis.org/A098721
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(n-1)/2)^(n-3) (a(2)=1, a(3)=1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the offset 2 the exponent would be negative; return 1. */
    if (n <= 3)
        return (seq_t)1;

    /* Compute the base C(n,2) avoiding overflow of the index type. */
    seq_t base = ((seq_t)n * (seq_t)(n - 1)) / 2;

    /* Exponent n-3 is strictly positive for n>=4. */
    idx_t exp = n - 3;

    /* Binary exponentiation using only allowed operators. */
    seq_t result = (seq_t)1;
    while (exp > 0) {
        /* if exp is odd multiply by current base */
        if (exp & 1U)
            result = result * base;
        exp >>= 1U;
        /* square the base for the next bit, but avoid an extra
           multiplication when exp becomes zero. */
        if (exp)
            base = base * base;
    }

    return result;
}
