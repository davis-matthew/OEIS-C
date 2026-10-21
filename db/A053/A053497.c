/*
 * A053497  Number of degree-n permutations of order dividing 7.
 * https://oeis.org/A053497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/7⌋} n!/(7^k*k!*(n-7k)!) Equivalent
 *            recurrence: a(n) = a(n-1) +
 *            (n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*a(n-7) for n >= 7, with
 *            initial values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t buf[8];
    /* initialise the first seven terms */
    for (idx_t i = 0; i <= 6 && i <= n; ++i)
        buf[i] = 1;
    if (n < 7) return 1;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t prod = (seq_t)(i - 1);
        prod *= (i - 2);
        prod *= (i - 3);
        prod *= (i - 4);
        prod *= (i - 5);
        prod *= (i - 6);
        seq_t term = prod * buf[(i - 7) & 7];
        buf[i & 7] = buf[(i - 1) & 7] + term;
    }
    return buf[n & 7];
}
