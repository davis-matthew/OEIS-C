/*
 * A087301  a(n) = n!*Sum_{i=1..n-1} (-1)^(i+1)/i.
 * https://oeis.org/A087301
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{i=1}^{n-1} (-1)^{i+1}/i Recurrence (n ≥ 3): a(n)
 *            = n·a(n‑1) + (-1)^n · n · (n‑2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 2 (offset 2) */

/* Seq_t and idx_t are predefined in the compilation environment. */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined below the offset */

    seq_t fact = 2;               /* 2! at n = 2   */
    seq_t res  = fact;            /* a(2) = 2      */
    if (n == 2) return res;

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t prev_fact = fact;               /* (k‑1)!       */
        fact = prev_fact * (seq_t)k;           /* k!           */

        /* (k‑2)! = prev_fact / (k‑1) */
        seq_t t = prev_fact / (seq_t)(k - 1);
        t = t * (seq_t)k;                     /* k·(k‑2)!     */

        if (k & 1)            /* k is odd: subtract the term   */
            res = (seq_t)k * res - t;
        else                  /* k is even: add the term      */
            res = (seq_t)k * res + t;
    }
    return res;
}
