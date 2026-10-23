/*
 * A162814  a(n) = 6*a(n-1)-6*a(n-2) for n > 1; a(0) = 5, a(1) = 18.
 * https://oeis.org/A162814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)-6*a(n-2) ; a(0)=5, a(1)=18
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)18;

    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)18;
    seq_t an = 0;

    for (idx_t k = 2; k <= n; ++k) {
        an = (seq_t)6 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
