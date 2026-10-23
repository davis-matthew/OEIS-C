/*
 * A168527  a(n) = n^6*(n^2 + 1)/2.
 * https://oeis.org/A168527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 * (n^2 + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the formula while avoiding overflow for all n
 * whose final value fits in the type seq_t. The algorithm
 * first divides an odd factor by 2 (either n^2 if n is even,
 * or (n^2 + 1) if n is odd) so that the intermediate products
 * remain small enough for seq_t of 64 bits.  For arbitrary
 * precision seq_t types the same arithmetic is correct. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* index as seq_t */
    seq_t sq = sn * sn;           /* n^2   (fits in seq_t for all n < 2^32) */
    seq_t sq3 = (sq * sq) * sn;   /* n^4 * n = n^5?  Not used below */

    seq_t a = sq;                 /* n^2 */
    seq_t b = sq + 1;             /* n^2 + 1 */

    if ((sn & 1) == 0) {          /* n is even -> divide n^2 by 2 */
        a /= 2;
    } else {                      /* n is odd -> divide n^2 + 1 by 2 */
        b /= 2;
    }

    /* compute (a * n^2 * n^2 * b) = n^6 * (n^2+1)/2 safely */
    seq_t res = a * sq;           /* a * n^2  */
    res = res * sq;               /* * n^2 again  (now a * n^4)   */
    res = res * b;                /* * final factor */
    return res;
}
