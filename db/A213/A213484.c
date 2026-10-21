/*
 * A213484  Number of (w,x,y) with all terms in {0,...,n} and |w-x| + |x-y| +
 *          |y-w| >= w+x+y.
 * https://oeis.org/A213484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + 3*n + 4 + 3*((n+1) mod 4 - (n+1) mod 2)) / 4
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+1) mod 4 and (n+1) mod 2 using bitwise operations */
    idx_t t = (n + 1) & 3;   /* matches % 4 */
    idx_t u = (n + 1) & 1;   /* matches % 2 */
    idx_t diff = t - u;      /* non‑negative: 0 or 2 */

    seq_t n_seq = (seq_t)n;
    /* numerator: 3*n^2 + 3*n + 4 + 3*diff */
    seq_t num = (seq_t)3 * n_seq * n_seq      /* 3*n^2   */
             + (seq_t)3 * n_seq              /* +3*n    */
             + (seq_t)4                     /* +4      */
             + (seq_t)3 * (seq_t)diff;      /* +3*diff */

    return num / (seq_t)4;
}
