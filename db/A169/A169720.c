/*
 * A169720  a(n) = (3*2^(n-1)-1)*(3*2^n-1).
 * https://oeis.org/A169720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^(n-1)-1)*(3*2^n-1). Recurrence: a(0)=1, a(1)=10,
 *            a(2)=55, a(n)=7*a(n-1) - 14*a(n-2) + 8*a(n-3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)55;
    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)10;  /* a(n-2)   */
    seq_t a2 = (seq_t)55;  /* a(n-1)   */
    seq_t a3;
    for (idx_t i = 3; i <= n; ++i) {
        a3 = 7*a2 - 14*a1 + 8*a0;  /* a(n) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
