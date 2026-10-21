/*
 * A220371  a(n) = Product_{i=1..2*n} (4*i+2)*A060818(n).
 * https://oeis.org/A220371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{2n} (4*i+2) * 2^{n - popcount(n)}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where popcount(n) is the number of 1–bits in the binary
 * representation of n.  The first term a(0) equals 1. */

seq_t a(idx_t n)
{
    /* compute the product of (4*i+2) for i = 1..2n */
    seq_t result = 1;
    for (idx_t i = 1; i <= 2 * n; ++i) {
        result *= (seq_t)(4 * i + 2);
    }

    /* compute n - popcount(n)  */
    idx_t pop = 0;
    idx_t tmp = n;
    while (tmp) {
        pop += tmp & 1;
        tmp >>= 1;
    }
    idx_t exp = n - pop;

    /* multiply by 2^exp */
    seq_t pow2 = 1;
    for (idx_t k = 0; k < exp; ++k) {
        pow2 *= 2;
    }
    result *= pow2;

    return result;
}
