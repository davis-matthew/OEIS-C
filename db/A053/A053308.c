/*
 * A053308  Partial sums of A053296.
 * https://oeis.org/A053308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + binomial(n+7,7); a(-1)=0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 30, u64 n <= 76, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (0‑based) of OEIS A053308: partial sums of
   A053296.  The function works for any integer type defined as
   seq_t (arbitrary‑precision or built‑in) and for any index type
   defined as idx_t (unsigned, 64‑bit). */
seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1. */
    if (n == 0) {
        return 1;
    }

    /* a(-1) = 0, a(0) = 1.  We keep the last two values. */
    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 1;   /* a(n-1) */

    /* binom holds C(i+7, 7) where i is the current n value. */
    seq_t binom = 1;   /* C(7,7) for i = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update binom to C(i+7,7) using the multiplicative formula:
           C(m,7) = C(m-1,7) * m / (m-7).  Here m = i+7. */
        binom = binom * (seq_t)(i + 7) / (seq_t)i;

        /* a(i) = a(i-1) + a(i-2) + C(i+7,7). */
        seq_t cur = prev1 + prev2 + binom;

        /* Shift the window for the next iteration. */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
