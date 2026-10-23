/*
 * A130259  Maximal index k of an even Fibonacci number (A001906) such that
 *          A001906(k) = Fib(2k) <= n (the 'lower' even Fibonacci Inverse).
 * https://oeis.org/A130259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal k with Fib(2k) ≤ n (Fibonacci at even indices).
 *            Implemented by iteratively computing Fibonacci numbers until the
 *            first value exceeding n is reached and returning the largest
 *            even index divided by 2
 * Range:     u8 n <= 1129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)0;            /* F(0)   */
    seq_t curr = (seq_t)1;            /* F(1)   */
    idx_t i = 0;                      /* current Fibonacci index */
    idx_t last_k = 0;                 /* corresponding k = i/2 */

    /* F(0)=0 is always ≤ n for n≥0 */
    last_k = 0;

    while (1) {
        seq_t next = prev + curr;     /* F(i+1) */
        prev = curr;                  /* shift */
        curr = next;
        i++;                          /* now prev is F(i) */

        if (prev > (seq_t)n) break;   /* first > n → stop */

        if ((i & 1) == 0)             /* even index => Fibonacci at 2k */
            last_k = i / 2;           /* k = i/2 */
    }

    return (seq_t)last_k;
}
