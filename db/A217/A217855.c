/*
 * A217855  Numbers m such that 16*m*(3*m+1)+1 is a square.
 * https://oeis.org/A217855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 2, with a(0)=0, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)2;
    seq_t a_nm2 = (seq_t)0;          /* a(n-2) for next iteration */
    seq_t a_nm1 = (seq_t)2;          /* a(n-1) for next iteration */
    seq_t a_n   = 0;
    for (idx_t i = 2u; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)14 - a_nm2 + (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
