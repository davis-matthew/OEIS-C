/*
 * A022364  Fibonacci sequence beginning 0, 30.
 * https://oeis.org/A022364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 * A000045(n) (A000045(n) is the Fibonacci number with
 *            F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute Fibonacci F(n) iteratively */
    seq_t f_prev = (seq_t)0;   /* F(0) */
    seq_t f_curr = (seq_t)1;   /* F(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Multiply by 30 */
    return f_curr * ((seq_t)30);
}
