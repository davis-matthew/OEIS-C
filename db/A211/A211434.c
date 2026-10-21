/*
 * A211434  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w+2x+5y=0.
 * https://oeis.org/A211434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n*(n+1) + c(n)) / 5 where c(n) = 5, if n % 5 == 0 or 4
 *            -3, if n % 5 == 1 or 3 1, if n % 5 == 2 The sequence is: 1, 1,
 *            5, 9, 17, 25,
 * Range:     u8 n <= 7, u16 n <= 192, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index into seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* 4 * n * (n + 1)  (fits in seq_t for all tested n). */
    seq_t t = 4 * nn * (nn + 1);

    /* Integer division by 5 gives the base value; remainder needed for the
       small adjustment depending on n mod 5. */
    seq_t q     = t / 5;           /* floor(t/5) */
    int     rem = (int)(t % 5);    /* t mod 5, fits in 3 bits */

    /* Precomputed c(n) values for each residue of n modulo 5. */
    static const int cvals[5] = {5, -3, 1, -3, 5};

    idx_t mod5 = n % 5;            /* 0 .. 4 */

    /* The adjustment (c + rem) is always 0 or 5; divide by 5 gives 0 or 1. */
    int adj = (cvals[mod5] + rem) / 5;

    /* Final result.  adj is 0 or 1, so it fits in seq_t easily. */
    return q + (seq_t)adj;
}
