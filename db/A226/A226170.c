/*
 * A226170  The number of cycles over all even permutations of {1,2,...,n}.
 * https://oeis.org/A226170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! * H_n + (-1)^(n+1) * (n-2)!)/2 where H_n = 1 + 1/2 +
 *            … + 1/n. Equivalently: if n is even: a(n) = (n! * H_n + (n-2)!)
 *            / 2 if n is odd : a(n) = (n! * H_n - (n-2)!) / 2 The
 *            implementation uses only integer arithmetic (no floats, no
 *            <math.h>). It computes n! first, then the sum n! / i for...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (a(0) = 0, a(1) = 1) explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;          /* fact now holds n! */

    /* Compute sum_T = sum_{i=1}^n fact / i,
       which equals n! * H_n.  Use an iterative relation to avoid
       repeated division of the large factorial. */
    seq_t sum = 0;
    seq_t term = fact;              /* term = fact / i   (start with i = 1) */
    for (idx_t i = 1; i <= n; ++i)
    {
        sum += term;
        if (i < n)                  /* prepare term for the next i+1 */
            term = term * (seq_t)i / (seq_t)(i + 1);
    }

    /* Compute (n-2)!  as fact / (n * (n-1)).  All divisions are exact. */
    seq_t D = fact;
    D /= (seq_t)n;
    D /= (seq_t)(n - 1);

    /* Apply the parity rule.  The division by 2 is exact. */
    if ((n & 1) == 0)   /* n even */
        return (sum + D) / 2;
    else                /* n odd */
        return (sum - D) / 2;
}
