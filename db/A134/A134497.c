/*
 * A134497  a(n) = Fibonacci(6n+5).
 * https://oeis.org/A134497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(6*n + 5) (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* target Fibonacci index */
    idx_t k = n * 6 + 5;

    /* Small‐index hand for speed */
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;

    /* Iterative fast‑doubling would be overkill for k ≤ 60005 (n ≤ 10000) */
    seq_t f_prev = 0;   /* F_0 */
    seq_t f_curr = 1;   /* F_1 */

    for (idx_t i = 2; i <= k; ++i) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return f_curr;   /* F_k */
}
