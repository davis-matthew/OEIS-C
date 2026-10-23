/*
 * A010924  Pisot sequence E(8,55), a(n) = floor(a(n-1)^2/a(n-2) + 1/2).
 * https://oeis.org/A010924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)+6*a(n-2), a(0)=8, a(1)=55
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)8;
    if (n == 1u) return (seq_t)55;

    seq_t a0 = (seq_t)8;
    seq_t a1 = (seq_t)55;
    seq_t an;

    for (idx_t i = 2u; i <= n; ++i) {
        an = 6 * a1 + 6 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
