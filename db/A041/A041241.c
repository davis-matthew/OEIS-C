/*
 * A041241  Denominators of continued fraction convergents to sqrt(132).
 * https://oeis.org/A041241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 46*a(n-2) - a(n-4). a(0)=1, a(1)=2, a(2)=45, a(3)=92
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)45;
    if (n == 3) return (seq_t)92;

    /* Use a ring buffer of size 4 to hold a(n-4), a(n-3),
       a(n-2), and a(n-1). */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0)   */
    buf[1] = (seq_t)2;   /* a(1)   */
    buf[2] = (seq_t)45;  /* a(2)   */
    buf[3] = (seq_t)92;  /* a(3)   */

    /* Compute terms from 4 up to n. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Indices modulo 4. */
        idx_t idx   = i          & 3;   /* = i % 4   */
        idx_t idx2  = (i - 2)    & 3;   /* = (i-2)%4 */
        idx_t idx4  = (i - 4)    & 3;   /* = (i-4)%4 */

        /* recurrence: a(i) = 46*a(i-2) - a(i-4) */
        buf[idx] = (seq_t)46 * buf[idx2] - buf[idx4];
    }

    /* After the loop, a(n) is stored at position n mod 4. */
    return buf[n & 3];
}
