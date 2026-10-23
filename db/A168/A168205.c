/*
 * A168205  a(n) = 4*n - a(n-1) + 1 with n>1, a(1)=2.
 * https://oeis.org/A168205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3 + 3*(-1)^n)/2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: if n is odd: a(n)=2*n; if n is even: a(n)=2*n+3 */
seq_t a(idx_t n)
{
    seq_t two_n = (seq_t)n * 2;
    if ((n & 1) == 0) {
        return two_n + 3;
    } else {
        return two_n;
    }
}
