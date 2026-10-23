/*
 * A130238  Partial sums of A130237.
 * https://oeis.org/A130238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)*A130233(n) - (Fib(A130233(n)) -
 *            1)*(Fib(A130233(n) + 1) - 1))/2
 * Range:     u8 n <= 7, u16 n <= 112, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130233(n) = index of the largest Fibonacci number <= n (with F_0 = 0, F_1 = 1) */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 0;

    /* Find i = A130233(n) and the corresponding Fibonacci numbers */
    seq_t cur  = 0;          /* F_i            */
    seq_t next = 1;          /* F_{i+1}        */
    idx_t  i   = 0;          /* current index  */
    seq_t  nseq = (seq_t)n;  /* convert index to seq_t once   */

    while (next <= nseq) {
        ++i;                     /* move to next index  */
        seq_t tmp = cur + next;  /* F_{i+2} = F_i + F_{i+1} */
        cur  = next;             /* shift window */
        next = tmp;
    }

    /* Compute the expression from the formula */
    seq_t term1   = nseq * (nseq + 1) * (seq_t)i;
    seq_t term2   = (cur - 1) * (next - 1);
    seq_t numerator = term1 - term2;

    return numerator / 2;
}
