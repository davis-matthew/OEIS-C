/*
 * A025956  Expansion of 1/((1-2*x)*(1-3*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A025956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 236*a(n-2) + 732*a(n-3) - 720*a(n-4); Initial
 *            values: a(0)=1, a(1)=27, a(2)=493, a(3)=7671
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)493;
    if (n == 3) return (seq_t)7671;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)27;
    seq_t a2 = (seq_t)493;
    seq_t a3 = (seq_t)7671;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)27 * a3 - (seq_t)236 * a2 + (seq_t)732 * a1 - (seq_t)720 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
