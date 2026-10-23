/*
 * A022090  Fibonacci sequence beginning 0, 7.
 * https://oeis.org/A022090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=7; a(n)=a(n-1)+a(n-2) for n>=2 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)7;

    /* Use two previous terms to iteratively compute a(n) */
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)7;   /* a(1) */
    seq_t current;

    /* Iterate from i = 2 up to n, inclusive */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        current = prev1 + prev2;  /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;   /* a(n) is stored in prev1 after loop */
}
