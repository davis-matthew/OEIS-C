/*
 * A144181  INVERT transform of A118434, = row sums of triangle A144182.
 * https://oeis.org/A144181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-3) for n>3, a(0)=1, a(1)=1, a(2)=3, a(3)=9
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)3;
    if (n == 3u) return (seq_t)9;

    seq_t m0 = (seq_t)1; /* a(1) */
    seq_t m1 = (seq_t)3; /* a(2) */
    seq_t m2 = (seq_t)9; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = m2 + (seq_t)2 * m0;
        m0 = m1;
        m1 = m2;
        m2 = next;
    }
    return m2;
}
