/*
 * A073554  Number of Fibonacci numbers F(k), k <= 10^n, which end in 7.
 * https://oeis.org/A073554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A073554: Number of Fibonacci numbers F(k), k <= 10^n, ending in 7.
 *
 * The sequence satisfies the linear recurrence
 *
 *     a(1) = 0
 *     a(2) = 14
 *     a(3) = 134
 *     a(n) = 11*a(n-1) - 10*a(n-2)   for n > 3
 *
 * This recurrence follows directly from the closed form  
 * a(n) = (2*10^n + 10)/15 for n > 1.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)14;
    if (n == (idx_t)3) return (seq_t)134;

    /* For n >= 4 compute iteratively using the recurrence. */
    seq_t prev2 = (seq_t)14;    /* a(2) */
    seq_t prev1 = (seq_t)134;   /* a(3) */
    seq_t cur;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        cur = prev1 * (seq_t)11 - prev2 * (seq_t)10;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
