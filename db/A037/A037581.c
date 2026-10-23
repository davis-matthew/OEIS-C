/*
 * A037581  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3.
 * https://oeis.org/A037581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-2) - 9*a(n-3); a(1)=1, a(2)=12, a(3)=109
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)109;
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)12;  /* a(n-2) */
    seq_t a3 = (seq_t)109; /* a(n-1) */
    seq_t ai;
    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 * (seq_t)9 + a2 - a1 * (seq_t)9;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
