/*
 * A131631  Supersubfactorials: partial product of positive subfactorials
 *          (A000166).
 * https://oeis.org/A131631
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=2..n} !k where !k is the subfactorial (derangement)
 *            of k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;  /* undefined for n < 2; the function is intended for n ≥ 2 */
    }
    seq_t prod = 1;
    /* subfactorial values */
    seq_t d0 = 1;   /* !0 = 1 */
    seq_t d1 = 0;   /* !1 = 0 */

    for (idx_t k = 2; k <= n; ++k) {
        /* !k = (k-1) * ( !(k-1) + !(k-2) ) */
        seq_t newd = (seq_t)(k - 1) * (d1 + d0);
        d0 = d1;
        d1 = newd;
        prod *= newd;  /* accumulate product of subfactorials */
    }
    return prod;
}
