/*
 * A109044  a(n) = lcm(n,3).
 * https://oeis.org/A109044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,3) = 3 * n / gcd(n,3)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since gcd(n,3) is 3 when n ≡ 0 (mod 3) and 1 otherwise, the
   function is simply: a(n) = n if 3 | n; otherwise a(n) = 3*n.
   a(0) = 0.  No external headers or helper functions are required. */

seq_t a(idx_t n)
{
    /* Handle the offset value a(0) = 0. */
    if (n == 0u) {
        return (seq_t)0;
    }

    /* If n is a multiple of 3, lcm(n,3) equals n; otherwise it equals 3*n. */
    if ((n % 3u) == 0u) {
        return (seq_t)n;
    } else {
        return (seq_t)n * (seq_t)3;
    }
}
