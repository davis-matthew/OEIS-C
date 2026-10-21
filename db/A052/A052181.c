/*
 * A052181  Partial sums of A050483.
 * https://oeis.org/A052181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)*C(n+7,7)/2 (OEIS A052181)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 46, u64 n <= 806,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+7,7)  multiplicatively, using 128‑bit intermediates
       to avoid overflow for 64‑bit seq_t.  The result is guaranteed to
       fit in seq_t for all n for which the final value does.               */
    unsigned __int128 binom = 1;
    idx_t i;

    for (i = 1; i <= 7; ++i) {
        /* binom = binom * (n + 8 - i) / i   */
        binom = binom * ((unsigned __int128)n + (unsigned __int128)(8 - i)) / (unsigned __int128)i;
    }

    /* a(n) = binom * (n+2) / 2  */
    binom = binom * ((unsigned __int128)n + 2);
    binom = binom / 2;

    return (seq_t)binom;
}
