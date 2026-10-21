/*
 * A227438  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having two adjacent 1's and two
 *          adjacent 0's.
 * https://oeis.org/A227438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 6*a(n-3) Initial values: a(1)=4,
 *            a(2)=16, a(3)=62
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)   return (seq_t)0;
    if (n == 1)   return (seq_t)4;
    if (n == 2)   return (seq_t)16;
    if (n == 3)   return (seq_t)62;

    seq_t a1 = 4;   /* a(n-3) */
    seq_t a2 = 16;  /* a(n-2) */
    seq_t a3 = 62;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t k = 4; k <= n; ++k) {
        an = (seq_t)4 * a3 - (seq_t)2 * a2 + (seq_t)6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
