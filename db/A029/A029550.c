/*
 * A029550  Highest minimal norm for an (even or odd) 2-modular lattice in
 *          dimension n.
 * https://oeis.org/A029550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=1, a(n)=2*floor(n/4) for n>=4
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;
    return (seq_t)2 * (seq_t)(n / 4);
}
