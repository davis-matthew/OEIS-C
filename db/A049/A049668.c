/*
 * A049668  a(n) = Fibonacci(8*n)/21.
 * https://oeis.org/A049668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 47*a(n-1)-a(n-2)  with a(0)=0, a(1)=1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t prev2 = 0; /* a(0) */
    seq_t prev1 = 1; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 47 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
