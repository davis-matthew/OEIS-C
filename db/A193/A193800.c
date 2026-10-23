/*
 * A193800  Least m > 0 such that (n+m)^2 - m^2 (= n^2 + 2*m*n) is a square.
 * https://oeis.org/A193800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > 0 such that n^2 + 2*n*m is a perfect square
 * Range:     u8 n <= 6, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* In terms of k we need the smallest integer k > n with (k^2 - n^2) divisible
       by 2n.  Let a = k - n, then a*(a + 2n) must be a multiple of 2n.
       The minimal a is found by a simple search up to 2n (since a = 2n is
       always a solution).  Once a is found, m = (a*(a+2n))/(2n). */
    seq_t two_n = (seq_t)n * 2;
    seq_t a_val = 1;

    for (;; ++a_val) {
        seq_t prod = a_val * (a_val + two_n);
        if ((prod % two_n) == 0) {
            seq_t m = prod / two_n;
            return m;
        }
    }
}
