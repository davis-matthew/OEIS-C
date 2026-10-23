/*
 * A145608  Numbers a(n)=k such that (1/3)*(5*(2k+1)^2-2) is A057080(n)^2.
 * https://oeis.org/A145608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 8*a(n+1) - a(n) + 3, with a(0)=0, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A145608 */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    /* Iterative computation using the second‑order recurrence */
    seq_t prev2 = (seq_t)0;      /* a(0) */
    seq_t prev1 = (seq_t)3;      /* a(1) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)8 * prev1 - prev2 + (seq_t)3;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
