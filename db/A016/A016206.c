/*
 * A016206  Expansion of 1/((1-x)*(1-2x)*(1-11x)).
 * https://oeis.org/A016206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 2^(n+1) – 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t ax = (seq_t)1;           /* a(0) */
    seq_t pow2 = (seq_t)4;         /* 2^(1+1) for the first iteration */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        ax = ax * (seq_t)11 + pow2 - (seq_t)1;
        pow2 = pow2 * (seq_t)2;   /* prepare 2^(i+2) for next step */
    }

    return ax;
}
