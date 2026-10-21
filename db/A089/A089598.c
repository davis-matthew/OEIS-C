/*
 * A089598  G.f.: (1+x^2+x^3)/(1-x^3)^2.
 * https://oeis.org/A089598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n ≥ 6 with initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=0, a(5)=2
 * Range:     u8 n <= 383, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)0;
    if (n == 5) return (seq_t)2;

    seq_t buf[6] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)0, (seq_t)2 };

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * buf[(i - 3) % 6] - buf[(i - 6) % 6];
        buf[i % 6] = next;
    }
    return buf[n % 6];
}
