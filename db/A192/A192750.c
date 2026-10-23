/*
 * A192750  Define a pair of sequences c_n, d_n by c_0=0, d_0=1 and thereafter
 *          c_n = c_{n-1}+d_{n-1}, d_n = c_{n-1}+4*n+2; sequence here is d_n.
 * https://oeis.org/A192750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  c_0 = 0, d_0 = 1;
   for n > 0:  c_n = c_{n-1} + d_{n-1},
               d_n = c_{n-1} + 4*n + 2
   Sequence a(n) is d_n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* d_0 = 1 */

    seq_t c = (seq_t)0;           /* c_0 */
    seq_t d = (seq_t)1;           /* d_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute next pair (c_i, d_i) using previous values */
        seq_t c_new = c + d;                      /* c_i = c_{i-1} + d_{i-1} */
        seq_t d_new = c + (seq_t)(4ULL * i + 2ULL);/* d_i = c_{i-1} + 4*i + 2 */

        c = c_new;
        d = d_new;
    }

    return d;   /* after loop: d holds d_n */
}
