/*
 * A055819  Row sums of array T in A055818; twice the odd-indexed Fibonacci
 *          numbers after initial term.
 * https://oeis.org/A055819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) (n >= 3) Initial values: a(0)=1,
 *            a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Compute subsequent terms iteratively */
    seq_t prev_prev = (seq_t)2;   /* a(n-2)  */
    seq_t prev      = (seq_t)4;   /* a(n-1)  */
    seq_t current = 0;            /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        /* current = 3*prev - prev_prev */
        current = prev;
        current += prev;          /* 2*prev   */
        current += prev;          /* 3*prev   */
        current -= prev_prev;     /* 3*prev - prev_prev */

        /* Shift window for next iteration */
        prev_prev = prev;
        prev = current;
    }

    return current;
}
