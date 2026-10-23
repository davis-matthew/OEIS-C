/*
 * A033190  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(Fibonacci(k)+1,2).
 * https://oeis.org/A033190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=3, a(3)=9, a(4)=28,
   a(n)=8*a(n-1)-21*a(n-2)+20*a(n-3)-5*a(n-4)  (n ≥ 5) */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0u) return 0;
    if (n == 1u) return 1;
    if (n == 2u) return 3;
    if (n == 3u) return 9;
    if (n == 4u) return 28;

    /* Pre‑compute the first four terms. */
    seq_t prev4 = 1;   /* a(1) */
    seq_t prev3 = 3;   /* a(2) */
    seq_t prev2 = 9;   /* a(3) */
    seq_t prev1 = 28;  /* a(4) */
    seq_t cur   = 0;   /* a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        cur   = 8 * prev1 - 21 * prev2 + 20 * prev3 - 5 * prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
