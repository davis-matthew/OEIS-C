/*
 * A034121  Fractional part of cube root of a(n) starts with digit 5.
 * https://oeis.org/A034121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034121:
   a(n) = the n-th positive integer m such that the fractional part of the cube root of m
   starts with the decimal digit 5, i.e. 0.5 ≤ {m^(1/3)} < 0.6.  The numbers are
   generated in increasing order.  The algorithm enumerates the ranges of m
   for each integer floor(cuberoot(m)) = k and finds the n-th element by counting
   how many integers lie in each range. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0, but safe guard */
        return 0;

    seq_t n_seq = (seq_t)n;      /* convert n to seq_t for arithmetic */
    seq_t cum    = 0;            /* cumulative count of qualifying numbers */

    for (idx_t k = 1; ; ++k)    /* k = floor(cuberoot(m)) */
    {
        /* lower = ceil(((2k+1)^3) / 8)  = ceil((k+0.5)^3) */
        seq_t t  = (seq_t)(2 * k + 1);
        seq_t low = t * t * t;                    /* (2k+1)^3 */
        low = (low + (seq_t)7) / (seq_t)8;        /* ceil division by 8 */

        /* upper = floor(((5k+3)^3 - 1) / 125) = floor((k+0.6)^3) - 1, excluding the upper bound */
        seq_t u  = (seq_t)(5 * k + 3);
        seq_t up = u * u * u;                     /* (5k+3)^3 */
        up = (up - (seq_t)1) / (seq_t)125;        /* floor division by 125 */

        /* count of integers in this block */
        seq_t count = 0;
        if (up >= low)
            count = up - low + (seq_t)1;

        /* if the desired term lies in this block, return it */
        if (cum + count >= n_seq) {
            seq_t offset = n_seq - cum - (seq_t)1;
            return low + offset;                   /* k-th block's offset */
        }

        /* otherwise, skip this block and continue */
        cum += count;
    }
}
