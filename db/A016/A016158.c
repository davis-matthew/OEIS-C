/*
 * A016158  Expansion of g.f. 1/((1-4*x)*(1-11*x)).
 * https://oeis.org/A016158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 11·a(n‑1) + 4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;          /* a(0) */
    seq_t pow4 = (seq_t)4;         /* 4^1 for the first step */

    if (n == 0)
        return res;

    for (idx_t i = 1; i <= n; ++i) {
        res = (seq_t)11 * res + pow4;  /* a(i) */
        pow4 *= (seq_t)4;             /* advance 4^i to 4^(i+1) */
    }

    return res;
}
