/*
 * A152690  Partial sums of superfactorials (A000178).
 * https://oeis.org/A152690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n‑1)+SF(n‑1), where SF(k)=∏_{i=1}^k i!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n)=1+∑_{k=1}^{n‑1}SF(k). */

seq_t a(idx_t n)
{
    /* For n==1, the only term is 1. */
    if (n == 1) return (seq_t)1;

    /* Variables work with type seq_t so that arbitrary‑precision
       arithmetic is used when available. */
    seq_t sum   = (seq_t)0;   /* Sum of all superfactorials up to n‑1 */
    seq_t fact  = (seq_t)1;   /* Current factorial k!   */
    seq_t super = (seq_t)1;   /* Current superfactorial ∏_{i=1}^k i! */

    /* Loop over k = 1 .. n‑1.
       For k=1 super and fact are already 1.
       For k>1 we update factorial and superfactorial. */
    for (idx_t k = (idx_t)1; k <= n-1; ++k)
    {
        if (k > 1)
        {
            fact  *= (seq_t)k;      /* k! = (k-1)! * k */
            super *= fact;         /* ∏_{i=1}^k i! = ∏_{i=1}^{k-1} i! * k! */
        }
        sum += super;                /* Add SF(k) to the running sum */
    }

    /* a(n) = 1 + sum_{k=1}^{n‑1} SF(k) */
    return sum + (seq_t)1;
}
