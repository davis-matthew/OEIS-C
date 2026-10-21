/*
 * A230403  a(n) = the largest k such that (k+1)! divides n; the number of
 *          trailing zeros in the factorial base representation of n
 *          (A007623(n)).
 * https://oeis.org/A230403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = largest k such that (k+1)! divides n
 * Range:     u8 n <= 5249, u16 n <= 9525, u32 n <= 9525, u64 n <= 9525,
 *            u128 n <= 9525, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined case – safe fallback */

    idx_t m   = 1;                       /* m! will be kept in fac */
    seq_t fac = 1;                       /* current factorial m! as seq_t */
    seq_t nm  = (seq_t)n;                /* n converted to seq_t for computations */

    for (;;) {
        idx_t inc = m + 1;              /* next factorial level (m+1)! */
        /* if multiplying fac by inc would exceed n, stop immediately */
        if (fac > nm / inc) break;

        seq_t next = fac * inc;         /* (m+1)! */
        if (nm % next != 0) break;      /* not divisible – stop */

        fac = next;                     /* update factorial */
        m   = inc;                       /* move to next level */
    }

    return (seq_t)(m - 1);               /* a(n) = largest k with (k+1)! | n */
}
