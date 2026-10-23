/*
 * A174679  a(4n) = n^2. a(4n+1) = (4n-1)^2. a(4n+2) = (2n)^2. a(4n+3) =
 *          (4n+1)^2.
 * https://oeis.org/A174679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174679: a(4n) = n^2,
   a(4n+1) = (4n-1)^2,
   a(4n+2) = (2n)^2,
   a(4n+3) = (4n+1)^2.      */
seq_t a(idx_t n)
{
    /* m = floor(n/4) */
    seq_t m = (seq_t)(n >> 2);
    switch ((int)(n & 3)) {
    case 0: /* a(4n) = n^2 */
        return m * m;
    case 1: /* a(4n+1) = (4n-1)^2 */
        if (m == 0)
            return (seq_t)1;            /* (-1)^2 */
        {
            seq_t val = (seq_t)4 * m - (seq_t)1;
            return val * val;
        }
    case 2: /* a(4n+2) = (2n)^2 */
        {
            seq_t val = (seq_t)2 * m;
            return val * val;
        }
    case 3: /* a(4n+3) = (4n+1)^2 */
        {
            seq_t val = (seq_t)4 * m + (seq_t)1;
            return val * val;
        }
    }
    return 0; /* unreachable */
}
