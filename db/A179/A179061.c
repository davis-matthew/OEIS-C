/*
 * A179061  Number of non-attacking placements of 6 rooks on an n X n board.
 * https://oeis.org/A179061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 720 * binomial(n,6)^2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 72, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    /* compute C(n,6) multiplicatively */
    seq_t c = 1;
    for (idx_t k = 1; k <= 6; ++k) {
        c = c * (seq_t)(n - k + 1) / k;
    }

    seq_t r = c * c;
    r *= (seq_t)720;
    return r;
}
