/*
 * A038177  Number of unbranched mono-5-catapolyheptagons.
 * https://oeis.org/A038177
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(2) = 1, a(3) = 3 For n ≥ 4: a(n) = (n + 2) * 2^(2n – 7) (n
 *            even) a(n) = (n + 2) * 2^(2n – 7) + 2^(n – 4) (n odd) In the
 *            following implementation the powers of two are built by repeated
 *            left‑shifts to avoid undefined behaviour for small seq_t widths.
 *            For n = 2 and 3 the values are returned expli...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* 2^(2n-7)  */
    idx_t e1 = 2 * n - 7;
    seq_t pow1 = (seq_t)1;
    for (idx_t i = 0; i < e1; ++i)
        pow1 <<= 1;

    seq_t ans = (seq_t)(n + 2) * pow1;

    /* if n is odd add 2^(n-4) */
    if (n & 1) {
        idx_t e2 = n - 4;
        seq_t pow2 = (seq_t)1;
        for (idx_t i = 0; i < e2; ++i)
            pow2 <<= 1;
        ans += pow2;
    }
    return ans;
}
