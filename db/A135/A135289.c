/*
 * A135289  Row 10 of A038207.
 * https://oeis.org/A135289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   number of (n)-dimensional subcubes in a 10‑cube = a(n) = C(10,n)
 *            * 2^(10-n), a(n)=0 for n>10
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 10) return 0;

    /* compute binomial coefficient C(10,n) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (10 - i + 1) / i;
    }

    /* compute 2^(10-n) without shifting by >= width */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < 10 - n; ++i) {
        pow2 <<= 1;  /* shift by 1, safe for all unsigned types */
    }

    return binom * pow2;
}
