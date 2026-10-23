/*
 * A109517  a(n) is the (1,2)-entry of the n-th power of the 2 X 2 matrix
 *          [0,1;n-1,2(n-1)].
 * https://oeis.org/A109517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n - 1 + sqrt(n*(n - 1)))^n - (n - 1 - sqrt(n*(n -
 *            1)))^n)/(2*sqrt(n*(n - 1)))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=2(n-1); for k>=3, a(k)=2(n-1)*a(k-1)+(n-1)*a(k-2) */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 1) return (seq_t)1;
    seq_t a_val = (seq_t)(n - 1);          /* a = n-1  */
    seq_t b_val = a_val * (seq_t)2;        /* b = 2(n-1) */

    seq_t y_prev2 = (seq_t)1;              /* y_1  */
    seq_t y_prev1 = b_val;                /* y_2  */

    if (n == 2) return y_prev1;

    /* Use the recurrence y_k = b*y_{k-1} + a*y_{k-2} */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t temp = y_prev1 * b_val + y_prev2 * a_val;
        y_prev2 = y_prev1;
        y_prev1 = temp;
    }
    return y_prev1;
}
