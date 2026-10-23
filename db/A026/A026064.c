/*
 * A026064  a(n) = (d(n)-r(n))/2, where d = A026063 and r is the periodic
 *          sequence with fundamental period (1,1,0,1).
 * https://oeis.org/A026064
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (d(n)-r(n))/2, where d = A026063 and r is the periodic
 *            sequence with fundamental period (1,1,0,1)
 * Range:     u8 none, u16 n <= 35, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 6.  For n < 6 return 0 (undefined). */
    if (n < 6)
        return 0;

    seq_t k = (seq_t)n - 6;            /* shift index to start at 0 */
    seq_t X = k + 6;                   /* (k + 6)          */
    seq_t Y = k * k + 30 * k + 71;     /* (k^2 + 30k + 71) */
    seq_t T = X * Y;                   /* product */
    seq_t A = (2 * T) / 3;             /* 8 * S  (exact integer division) */

    seq_t N = A - 3;                    /* start building the numerator */
    /* add (±1) corresponding to (-1)^k */
    if ((k & 1) == 0)
        N += 1;         /* +1 for even k */
    else
        N -= 1;         /* -1 for odd k */

    /* adjust for the cosine term: subtract 2*cos(kπ/2) */
    int r = (int)(k & 3);                /* k mod 4 */
    if (r == 0)      /* cos = 1 */
        N -= 2;
    else if (r == 2) /* cos = -1 */
        N += 2;
    /* r == 1 or 3: cos = 0, no change */

    return N / 8;                     /* final division by 8 gives the sequence value */
}
