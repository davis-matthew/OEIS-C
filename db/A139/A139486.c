/*
 * A139486  a(n) = Product_{j=0..n-1} (2^j + 2).
 * https://oeis.org/A139486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{j=0}^{n‑1} (2^j + 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */

    seq_t res = 1;                   /* result accumulator */
    seq_t p2  = 1;                   /* current power of two (2^j) */

    for (idx_t j = 0; j < n; ++j) {
        seq_t term = p2 + 2;         /* 2^j + 2 */
        res = res * term;            /* multiply into product */
        p2 <<= 1;                    /* next power of two: 2^(j+1) */
    }

    return res;
}
