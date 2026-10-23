/*
 * A153643  Jacobsthal numbers A001045 incremented by 2.
 * https://oeis.org/A153643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6 + 2^n - (-1)^n)/3 Computed via recurrence a(0) = 2
 *            a(1) = 3 a(2) = 3 a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) for n >= 3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return 2;
    if (n == 1 || n == 2) return 3;

    /* Iterate using only three temporary variables */
    seq_t a0 = 2;   /* a(n-3)  */
    seq_t a1 = 3;   /* a(n-2)  */
    seq_t a2 = 3;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* temp = 2*a2 + a1 */
        seq_t temp = a2 + a2;     /* 2*a2 */
        temp += a1;              /* +a1 */

        /* sub = 2*a0 */
        seq_t sub = a0 + a0;     /* 2*a0 */

        /* next = temp - sub */
        next = temp - sub;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;  /* a(n) */
}
