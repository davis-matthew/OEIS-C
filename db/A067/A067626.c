/*
 * A067626  a(n) = 2^(2n+1)*(2n+1)!.
 * https://oeis.org/A067626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n+1)*(2n+1)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 2*n + 1 as a seq_t to prevent overflow of idx_t. */
    idx_t tmp = 2 * n + 1;
    seq_t m = (seq_t)tmp;

    /* Factorial (2n+1)! */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= tmp; ++i) {
        res *= (seq_t)i;
    }

    /* Multiply by 2^(2n+1) = 2^m. */
    const seq_t two = (seq_t)2;
    for (idx_t i = (idx_t)0; i < tmp; ++i) {
        res *= two;
    }

    return res;
}
