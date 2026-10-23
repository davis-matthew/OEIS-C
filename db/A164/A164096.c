/*
 * A164096  Partial sums of A164095.
 * https://oeis.org/A164096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2)+11 (n > 2) * Initial values: a(1) = 5, a(2) = 11
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 249,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset starts at 1 – behaviour for n==0 is irrelevant */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)5;
    if (n == 2)
        return (seq_t)11;

    seq_t a_prev2 = (seq_t)5;    /* a(1)   */
    seq_t a_prev1 = (seq_t)11;   /* a(2)   */
    seq_t val = 0;

    for (idx_t i = 3; i <= n; ++i) {
        val = (seq_t)2 * a_prev2 + (seq_t)11; /* compute a(i) */
        a_prev2 = a_prev1;                   /* shift registers for next step */
        a_prev1 = val;
    }

    return a_prev1;   /* a(n) */
}
