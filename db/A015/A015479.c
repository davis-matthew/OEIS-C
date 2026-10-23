/*
 * A015479  q-Fibonacci numbers for q=7, scaling a(n-1).
 * https://oeis.org/A015479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=7^(n-1)*a(n-1)+a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A015479 (q‑Fibonacci 7). */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* a(i-2) and a(i-1) in successive iterations */
    seq_t a_prev = (seq_t)0;   /* a(0) */
    seq_t a_curr = (seq_t)1;   /* a(1) */

    /* 7^(i-1) starting value for i = 2 (since 7^(2-1)=7) */
    seq_t pow7 = (seq_t)7;

    /* Iterate from i = 2 up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = pow7 * a_curr + a_prev;   /* a(i) = 7^(i-1)*a(i-1) + a(i-2) */
        a_prev = a_curr;
        a_curr = term;
        pow7 = pow7 * (seq_t)7;                /* advance 7^(i) for next step */
    }

    return a_curr;
}
