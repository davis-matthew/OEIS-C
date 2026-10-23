/*
 * A046182  Indices of triangular numbers which are also octagonal.
 * https://oeis.org/A046182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-2) - a(n-4) + 48 Initial values: a(1) = 1 a(2) = 6
 *            a(3) = 153 a(4) = 638
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)6;
    if (n == (idx_t)3) return (seq_t)153;
    if (n == (idx_t)4) return (seq_t)638;

    seq_t a1 = (seq_t)1;        /* a(n-4) */
    seq_t a2 = (seq_t)6;        /* a(n-3) */
    seq_t a3 = (seq_t)153;      /* a(n-2) */
    seq_t a4 = (seq_t)638;      /* a(n-1) */
    seq_t an;

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        an = (seq_t)98 * a3 - a1 + (seq_t)48;
        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
