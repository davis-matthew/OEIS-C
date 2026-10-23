/*
 * A166336  Expansion of (1 - 4*x + 7*x^2 - 4*x^3 + x^4)/(1 - 7*x + 17*x^2 -
 *          17*x^3 + 7*x^4 - x^5).
 * https://oeis.org/A166336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 17*a(n-3) - 7*a(n-4) + a(n-5) *
 *            (for n ≥ 5). Initial values: a(0)=1, a(1)=3, a(2)=11, a(3)=39,
 *            a(4)=131
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)39;
    if (n == 4) return (seq_t)131;

    /* keep the five most recent values in a circular buffer */
    seq_t a0 = (seq_t)1;   /* a(n-5) for the first iteration */
    seq_t a1 = (seq_t)3;   /* a(n-4)   */
    seq_t a2 = (seq_t)11;  /* a(n-3)   */
    seq_t a3 = (seq_t)39;  /* a(n-2)   */
    seq_t a4 = (seq_t)131; /* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute the next term using only non‑negative intermediate values */
        seq_t pos = a4 * (seq_t)7 + a2 * (seq_t)17 + a0;  /* 7*a(n-1)+17*a(n-3)+a(n-5) */
        seq_t neg = a3 * (seq_t)17 + a1 * (seq_t)7;      /* 17*a(n-2)+7*a(n-4)   */
        seq_t next = pos - neg;                          /* guarantee pos ≥ neg */

        /* shift the window forward by one position */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;  /* a(n) is now in a4 */
}
