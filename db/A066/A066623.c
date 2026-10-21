/*
 * A066623  Concatenation of squares in increasing order up to the n-th and
 *          then in decreasing order.
 * https://oeis.org/A066623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of squares 1²,2²,…,n² followed by
 *            (n‑1)²,…,1²
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1; n≥1 normally */

    seq_t res = 0;

    /* increasing part 1² … n² */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;

        /* compute 10¹ᵈ where d = number of decimal digits of sq */
        seq_t pow10 = 1;
        seq_t tmp = sq;
        while (tmp > 0) {
            tmp /= 10;
            pow10 *= 10;
        }

        res = res * pow10 + sq;
    }

    /* decreasing part (n‑1)² … 1² */
    for (idx_t i = n; i > 1; --i) {
        seq_t sq = (seq_t)(i - 1) * (seq_t)(i - 1);

        seq_t pow10 = 1;
        seq_t tmp = sq;
        while (tmp > 0) {
            tmp /= 10;
            pow10 *= 10;
        }

        res = res * pow10 + sq;
    }

    return res;
}
