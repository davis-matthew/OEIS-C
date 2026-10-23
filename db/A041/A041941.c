/*
 * A041941  Denominators of continued fraction convergents to sqrt(493).
 * https://oeis.org/A041941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent to sqrt(493). Partial
 *            quotients of sqrt(493) are periodic: a0=22, block (a1..a9) =
 *            [4,1,10,3,3,10,1,4,44]. Use b_{-1}=0, b_0=1, b_n = a_n * b_{n-1}
 *            + b_{n-2}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* b_0 */

    /* Periodic part of the continued fraction (a1 .. a9) */
    static const seq_t block[9] = {
        (seq_t)4, (seq_t)1, (seq_t)10, (seq_t)3,
        (seq_t)3, (seq_t)10, (seq_t)1, (seq_t)4,
        (seq_t)44
    };

    seq_t b_prev2 = (seq_t)0;   /* b_{-1} */
    seq_t b_prev1 = (seq_t)1;   /* b_0  */
    seq_t b_curr, a_n;
    idx_t i;

    for (i = 1; i <= n; ++i) {
        /* a_i for i >= 1 */
        a_n = block[(i - 1) % 9];
        b_curr = a_n * b_prev1 + b_prev2;  /* b_i */
        b_prev2 = b_prev1;
        b_prev1 = b_curr;
    }

    return b_prev1;   /* b_n */
}
