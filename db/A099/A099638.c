/*
 * A099638  a(n) = A098210(n)/15.
 * https://oeis.org/A099638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3), a(1)=1, a(2)=77,
 *            a(3)=7437
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)77;
    if (n == 3) return (seq_t)7437;

    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)77;
    seq_t a3 = (seq_t)7437;
    seq_t temp;

    for (idx_t i = 4; i <= n; ++i) {
        temp = ((seq_t)111 * a3) - ((seq_t)1110 * a2) + ((seq_t)1000 * a1);
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }
    return a3;
}
