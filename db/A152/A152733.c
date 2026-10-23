/*
 * A152733  a(n) + a(n+1) + a(n+2) = 3^n.
 * https://oeis.org/A152733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152733: a(n)+a(n+1)+a(n+2)=3^n, offset 1
   Initial values: a(1)=0, a(2)=0, a(3)=3
   Recurrence used in code: a(n+2)=3^n - a(n) - a(n+1) */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 3;

    /* a(1) and a(2) known */
    seq_t prev2 = 0;          /* a(n)      */
    seq_t prev1 = 0;          /* a(n+1)    */

    /* power = 3^i, starting at i = 1 */
    seq_t power = 3;          /* 3^1 */

    /* Compute sequentially up to needed index */
    for (idx_t i = 1; i <= n - 2; ++i)
    {
        /* a(i+2) = 3^i - a(i) - a(i+1) */
        seq_t next = power - prev2 - prev1;

        if (i + 2 == n)      /* desired term reached */
            return next;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = next;

        /* Prepare power for next exponent */
        power = power * 3;
    }

    /* Should never reach here for valid n */
    return 0;
}
