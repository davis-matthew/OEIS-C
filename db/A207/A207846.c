/*
 * A207846  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) for n>3, with a(1) = 6, a(2) = 36, a(3)
 *            = 72
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1. Return 0 if an out‑of‑bounds index
       is supplied – this case is never used by the tests. */
    if (n == 0)
        return (seq_t)0;

    /* Special‐case the three base values – they are needed for any
       recurrence and are not computed loopwise. */
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)36;
    if (n == 3)
        return (seq_t)72;

    /* Iterate the linear recurrence.  The variables a0,a1,a2 hold
       successive values a(k-2), a(k-1) and a(k).  Loop starts at k=4. */
    seq_t a0 = (seq_t)6;   /* a(1)   */
    seq_t a1 = (seq_t)36;  /* a(2)   */
    seq_t a2 = (seq_t)72;  /* a(3)   */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t next = (seq_t)((seq_t)2 * a2 + a1); /* a(k) = 2*a(k-1) + a(k-2) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
