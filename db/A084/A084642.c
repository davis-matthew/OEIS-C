/*
 * A084642  A Jacobsthal ratio.
 * https://oeis.org/A084642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=1, a(3)=2, a(n)=a(n-2) for n>=4
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)3;
    }
    /* For n >= 2, the sequence alternates 1 on even indices and 2 on odd indices */
    return (n & (seq_t)1) ? (seq_t)2 : (seq_t)1;
}
