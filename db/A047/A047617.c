/*
 * A047617  Numbers that are congruent to {2, 5} mod 8.
 * https://oeis.org/A047617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (5 + (-1)^n)/2 For odd n: a(n) = 4*n - 2 For even
 *            n: a(n) = 4*n - 3 (a(1) = 2, a(2) = 5, a(3) = 10, …)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // n is >= 1; behaviour for n==0 is undefined but not required
    seq_t val = (seq_t)n;
    val *= 4;
    if (n & 1) {
        val -= 2;  // n is odd
    } else {
        val -= 3;  // n is even
    }
    return val;
}
