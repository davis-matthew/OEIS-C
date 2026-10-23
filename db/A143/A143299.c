/*
 * A143299  Number of terms in the Zeckendorf representation of every number
 *          in row n of the Wythoff array.
 * https://oeis.org/A143299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007895(n-1) + 1 where A007895(k) is the number of
 *            summands in the Zeckendorf (non‑consecutive Fibonacci)
 *            representation of k. We compute this by a greedy algorithm that
 *            repeatedly subtracts the largest Fibonacci number not exceeding
 *            the remaining value
 * Range:     u8 n <= 815, u16 n <= 9239, u32 n <= 9239, u64 n <= 9239,
 *            u128 n <= 9239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to Zeckendorf number n-1 (seq_t) */
    seq_t N = (seq_t)n - 1;          /* n≥1, so N≥0 */
    seq_t count = 0;                 /* number of summands found */

    /* Special case: n = 1 -> N = 0 -> representation has 0 summands,
       so a(1) = 1 */
    if (N == 0)
        return (seq_t)1;

    /* Greedy Zeckendorf decomposition */
    while (N > 0) {
        /* Fibonacci numbers starting with 1,2,3,5,... */
        seq_t f_prev = (seq_t)1;         /* F1 */
        seq_t f_curr = (seq_t)2;         /* F2 */

        /* Find largest Fibonacci ≤ N */
        while (f_curr <= N) {
            seq_t f_next = f_prev + f_curr;
            /* Detect unsigned overflow: if f_next < f_curr,
               the addition wrapped around, so we stop. */
            if (f_next <= f_curr)
                break;
            f_prev = f_curr;
            f_curr = f_next;
        }
        /* f_prev is now the largest Fibonacci ≤ N */
        N -= f_prev;
        ++count;
    }

    /* The sequence a(n) is the Zeckendorf summand count + 1 */
    return count + 1;
}
