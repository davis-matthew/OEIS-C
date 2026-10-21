/*
 * A099177  a(n) = 2*a(n-1) + 4*a(n-2) - 4*a(n-3) - 4*a(n-4).
 * https://oeis.org/A099177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=2, a(3)=8
   a(n)=2*a(n-1)+4*a(n-2)-4*a(n-3)-4*a(n-4)  for n≥4 */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)8;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)8;
    seq_t an = a3;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)2 * a3 + (seq_t)4 * a2 - (seq_t)4 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
