/*
 * A020878  Number of maximum matchings in the n-Moebius ladder M_n.
 * https://oeis.org/A020878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) + (n mod 2 ? 2 : 0) (Lucas(0)=2, Lucas(1)=1,
 *            Lucas(n)=Lucas(n-1)+Lucas(n-2)) Recurrence: a(n) =
 *            a(n-1)+2*a(n-2)-a(n-3)-a(n-4) for n>3 Initial values: a(0)=2,
 *            a(1)=3, a(2)=3, a(3)=6
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Lucas numbers: L(0)=2, L(1)=1 */
    seq_t l0 = 2;
    if (n == 0) return l0;
    seq_t l1 = 1;
    if (n == 1) return l1 + (seq_t)2;
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t l2 = l0 + l1;
        l0 = l1;
        l1 = l2;
    }
    if (n & 1) return l1 + (seq_t)2;
    return l1;
}
