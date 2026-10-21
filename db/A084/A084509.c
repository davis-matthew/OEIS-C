/*
 * A084509  Number of ground-state 3-ball juggling sequences of period n.
 * https://oeis.org/A084509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! for n ≤ 4; a(n) = 6·4^(n‑3) for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        seq_t res = 1;
        for (idx_t i = 2; i <= n; ++i)
            res *= (seq_t)i;
        return res;
    }
    /* for n ≥ 5: 4^(n-3) = 2^(2n-6) */
    idx_t shift = 2 * n - 6;
    seq_t val = ((seq_t)1) << shift;  /* 2^(2n-6) */
    return val * 6;
}
