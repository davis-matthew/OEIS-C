/*
 * A177412  Fibonacci sequence beginning 14831,41069.
 * https://oeis.org/A177412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 14831, a(1) = 41069
 * Range:     u8 none, u16 n <= 2, u32 n <= 25, u64 n <= 71, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0) return 14831;
    if (n == 1) return 41069;

    /* Recursively compute the sequence */
    seq_t a0 = 14831;   /* a(n-2) starts at n=2 */
    seq_t a1 = 41069;   /* a(n-1) starts at n=2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t temp = a1;            /* store current a(n-1) */
        a1 += a0;                   /* a(n) = a(n-1) + a(n-2) */
        a0 = temp;                  /* advance a0 to previous a(n-1) */
    }

    return a1;   /* a(n) now in a1 */
}
