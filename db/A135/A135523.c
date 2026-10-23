/*
 * A135523  a(n) = A007814(n) + A209229(n).
 * https://oeis.org/A135523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v₂(n) + { 1 if n is a power of two and n > 1, 0 otherwise
 *            } with v₂(n) = the exponent of the highest power of two dividing
 *            n. For n = 1, a(1) = 1 by definition
 * Range:     u8 n <= 8561, u16 n <= 8561, u32 n <= 8561, u64 n <= 8561,
 *            u128 n <= 8561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* special case */

    /* Count trailing zeros in n (i.e., v₂(n)) */
    idx_t m = n;
    uint64_t v2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        v2++;
    }

    seq_t result = (seq_t)v2;           /* first part: v₂(n) */

    /* add 1 if n is a power of two and n > 1 */
    if ((n & (n - 1)) == 0)             /* n is a power of two */
        result += (seq_t)1;

    return result;
}
