/*
 * A105225  a(n+3) = 2a(n+2) - 3a(n+1) + 2a(n); a(0) = 1, a(1) = -1, a(2) =
 *          -2.
 * https://oeis.org/A105225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+3) = 2a(n+2) - 3a(n+1) + 2a(n); a(0)=1, a(1)=-1, a(2)=-2
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return -1;
    if (n == 2) return -2;
    seq_t a0 = 1, a1 = -1, a2 = -2;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 2 * a2 - 3 * a1 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
