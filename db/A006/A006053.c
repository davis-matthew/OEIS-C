/*
 * A006053  a(n) = a(n-1) + 2*a(n-2) - a(n-3), with a(0) = a(1) = 0, a(2) = 1.
 * https://oeis.org/A006053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3), with a(0) = a(1) = 0, a(2) =
 *            1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 77, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t z = 0, y = 0, x = 1; /* a(i-2), a(i-1), a(i) for i = 2 */
    idx_t i;
    if (n < 2) return (seq_t)0;
    for (i = 2; i < n; i++) {
        seq_t t = x + 2 * y;
        t -= z;
        z = y;
        y = x;
        x = t;
    }
    return x;
}
