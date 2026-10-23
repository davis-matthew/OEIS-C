/*
 * A145889  Number of even entries that are followed by a smaller entry in all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A145889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A145889
    Formula: for n = 2k      :  a(n) = k^2 * (2k-1)!
                    for n = 2k+1  :  a(n) = k^2 * (2k)!
    a(0) = 0
*/
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 */
        return 0;

    /* k = floor(n/2) */
    idx_t k = n >> 1;
    seq_t k2 = (seq_t)k * (seq_t)k;          /* k^2 */

    /* factorial of m as seq_t */
    seq_t fact = 1;                    /* m! initialization       */
    idx_t m;
    if ((n & 1) == 0)                 /* even n: n = 2k   */
        m = (idx_t)(2 * k - 1);        /* m = 2k-1          */
    else                               /* odd  n: n = 2k+1 */
        m = (idx_t)(2 * k);            /* m = 2k            */

    for (idx_t i = 1; i <= m; ++i)
        fact *= (seq_t)i;              /* multiply by i       */

    return k2 * fact;                  /* a(n) = k^2 * m!    */
}
