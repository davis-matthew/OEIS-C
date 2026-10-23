/*
 * A046199  Indices of pentagonal numbers that are also heptagonal.
 * https://oeis.org/A046199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) – 63*a(n-2) + a(n-3) with initial values
 *            a(1)=1, a(2)=54, a(3)=3337
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)54;
    if (n == 3) return (seq_t)3337;

    /* Use a sliding window of the last three terms */
    seq_t a1 = (seq_t)1;       /* a(n-3) */
    seq_t a2 = (seq_t)54;      /* a(n-2) */
    seq_t a3 = (seq_t)3337;    /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)63 * a3 - (seq_t)63 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
