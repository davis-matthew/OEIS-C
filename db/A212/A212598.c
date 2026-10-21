/*
 * A212598  a(n) = n - m!, where m is the largest number such that m! <= n.
 * https://oeis.org/A212598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - m!, where m! is the largest factorial ≤ n
 * Range:     u8 n <= 374, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fac holds the current factorial value k!.
       m holds the last factorial that did not exceed n.          */
    seq_t fac = 1;
    seq_t m   = 1;
    seq_t k   = 1;               /* current k in k!                 */

    /* iterate while current factorial ≤ n, updating m each time   */
    while (fac <= (seq_t)n) {
        m = fac;                 /* m! ≤ n                           */
        ++k;                     /* next k                           */
        fac *= k;                /* compute (k+1)!                   */
    }

    /* return n - m! ; cast n to seq_t for exact subtraction    */
    return (seq_t)n - m;
}
