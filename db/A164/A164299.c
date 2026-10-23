/*
 * A164299  a(n) = ((1+4*sqrt(2))*(3+sqrt(2))^n +
 *          (1-4*sqrt(2))*(3-sqrt(2))^n)/2.
 * https://oeis.org/A164299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2); a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Iterative computation using the linear recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)11;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 6 * prev1;      /* 6 * a(n-1) */
        cur -= 7 * prev2;     /* minus 7 * a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
