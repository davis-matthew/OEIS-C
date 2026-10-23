/*
 * A008875  3x+1 sequence starting at 95.
 * https://oeis.org/A008875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th term of the Collatz (3x+1) sequence starting at
 *            95. The recurrence is: a(0)=95, a(n)=a(n-1)/2 if a(n-1) is even,
 *            otherwise a(n)=3*a(n-1)+1
 * Range:     u8 n <= 0, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)95;
    for (idx_t i = 1; i <= n; ++i) {
        if ((x & (seq_t)1) == (seq_t)0) {
            x = x >> (idx_t)1;
        } else {
            /* compute 3*x+1 using a larger intermediate type
               to avoid overflow for 8/16/32-bit seq_t */
            unsigned long long tmp = 3ULL * (unsigned long long)x + 1ULL;
            x = (seq_t)tmp;
        }
    }
    return x;
}
