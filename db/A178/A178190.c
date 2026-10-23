/*
 * A178190  Sum 7^((k^2+3k)/2) from k=1 to n.
 * https://oeis.org/A178190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 7^{(k^2+3k)/2}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1 using the recurrence
 *   term_1 = 7^2
 *   term_k = term_{k-1} * 7^{k+1}   (for k >= 2)
 *   a(n) = a(n-1) + term_n
 * This avoids repeated exponentiation and keeps the code
 * independent of the underlying integer type seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */

    /* term_1 = 49, a(1) = 49 */
    seq_t term = (seq_t)7 * (seq_t)7;     /* 7^2 */

    seq_t sum = term;
    if (n == 1) return sum;              /* first term only */

    /* mul holds 7^{k+1} for the current k.
       For k=2 the needed factor is 7^{3} = 343. */
    seq_t mul = (seq_t)7 * (seq_t)7 * (seq_t)7; /* 7^3 */

    for (idx_t k = 2; k <= n; ++k)
    {
        term = term * mul;      /* term_k = term_{k-1} * 7^{k+1} */
        sum += term;            /* accumulate */
        mul *= (seq_t)7;        /* next factor: 7^{k+2} */
    }

    return sum;
}
