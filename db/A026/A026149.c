/*
 * A026149  Expansion of 1/((1-2x)(1-5x)(1-10x)(1-11x)).
 * https://oeis.org/A026149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 28*a(n-1) - 267*a(n-2) + 980*a(n-3) - 1100*a(n-4)
   Initial terms:
   a(0) = 1, a(1) = 28, a(2) = 517, a(3) = 7980 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)517;
    if (n == 3) return (seq_t)7980;

    /* Use the recurrence for n >= 4 */
    seq_t a0 = (seq_t)1;      /* a(n-4)  */
    seq_t a1 = (seq_t)28;     /* a(n-3)  */
    seq_t a2 = (seq_t)517;    /* a(n-2)  */
    seq_t a3 = (seq_t)7980;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term:
           a(i) = 28*a(i-1) - 267*a(i-2) + 980*a(i-3) - 1100*a(i-4) */
        seq_t pos = (28 * a3) + (980 * a1);      /* positive part   */
        seq_t neg = (267 * a2) + (1100 * a0);    /* negative part   */
        next = pos - neg;                      /* no underflow: pos >= neg for all n */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
