/*
 * A168345  a(n) = n^4*(n^9 + 1)/2.
 * https://oeis.org/A168345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 31, u128 n <= 969,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;

    /* compute powers of n */
    seq_t n2  = x * x;          /* n^2 */
    seq_t n4  = n2 * n2;        /* n^4 */
    seq_t n8  = n4 * n4;        /* n^8 */
    seq_t n9  = n8 * x;         /* n^9 */

    /* compute a(n) while avoiding intermediate overflow */
    if (sizeof(seq_t) <= 8) {
        /* intermediate fits into 128‑bit, safe for n up to 32 */
        __uint128_t prod = (__uint128_t)n4 * (__uint128_t)(n9 + 1);
        prod /= 2;
        return (seq_t)prod;
    } else {
        /* seq_t is large enough (e.g., 128‑bit or arbitrary‑precision) */
        seq_t temp = n9 + 1;
        seq_t prod = n4 * temp;
        prod /= 2;
        return prod;
    }
}
