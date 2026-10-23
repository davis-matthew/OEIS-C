/*
 * A016133  Expansion of 1/((1-2*x)*(1-9*x)).
 * https://oeis.org/A016133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 2^n; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This matches the generating function 1/((1-2x)(1-9x)) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t prev = (seq_t)1;  /* a(0) */
    seq_t pow2 = (seq_t)1;  /* 2^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 *= (seq_t)2;          /* 2^i */
        seq_t cur = (seq_t)9 * prev + pow2;
        prev = cur;
    }

    return prev;
}
