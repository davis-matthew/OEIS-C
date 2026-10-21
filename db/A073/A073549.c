/*
 * A073549  Number of Fibonacci numbers F(k), k <= 10^n, which end in 6.
 * https://oeis.org/A073549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2), a(1)=0, a(2)=6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from a(n) = (10^n - 10)/15, and yields the number
   of Fibonacci numbers F(k) with k ≤ 10^n that end in 6. The implementation
   works for 8/16/32/64‑bit unsigned types as well as arbitrary‑precision
   integer types provided by the build. */

/* External linkage is required; no static keyword */
seq_t a(idx_t n)
{
    /* Handle the offset (a(1)=0) */
    if (n <= 1)
        return (seq_t)0;   /* n == 1 or below -- return 0 */

    /* Base cases */
    seq_t a_prev2 = (seq_t)0;          /* a(1) */
    seq_t a_prev1 = (seq_t)6;          /* a(2) */
    if (n == 2)
        return a_prev1;

    /* Iteratively compute up to a(n) */
    seq_t a_cur;
    for (idx_t i = 3; i <= n; ++i) {
        a_cur = (seq_t)11 * a_prev1 - (seq_t)10 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
