/*
 * A024875  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k =
 *          floor( n/2 ), s = natural numbers >= 3.
 * https://oeis.org/A024875
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3+30*n^2+76*n)/24 for n even,
 *            (2*n^3+27*n^2+46*n-75)/24 for n odd
 * Range:     u8 none, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 12, a(3) = 15, */
seq_t a(idx_t n) {
    /* n ≥ 2 is assumed */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t num;
    if ((n & 1) == 0) {          /* even n */
        num = (seq_t)2 * n3 + (seq_t)30 * n2 + (seq_t)76 * ns;
    } else {                    /* odd n */
        num = (seq_t)2 * n3 + (seq_t)27 * n2 + (seq_t)46 * ns - (seq_t)75;
    }
    return num / (seq_t)24;      /* division is exact */
}
