/*
 * A122883  The (1,3)-entry in the 3 X 3 matrix M^n, where M = [1,1,1 / 4,2,1
 *          / 9,3,1].
 * https://oeis.org/A122883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 11*a(n-2) - 2*a(n-3) Initial values: a(1)=1,
 *            a(2)=3, a(3)=23
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 23;
    seq_t a1 = 1;   /* a(n-3) */
    seq_t a2 = 3;   /* a(n-2) */
    seq_t a3 = 23;  /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = 4 * a3 + 11 * a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
