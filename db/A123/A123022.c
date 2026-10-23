/*
 * A123022  a(n) = n!*b(n) where b(n) = (n-4)*b(n-2)/(n*(n-1)) and b(0) = b(1)
 *          = 1.
 * https://oeis.org/A123022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-4)*a(n-2) with a(0)=1, a(1)=1
 * Range:     i8 n <= 12, i16 n <= 16, i32 n <= 24, i64 n <= 38, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the pre‑computed base values. */
    if (n == 0 || n == 1)
        return 1;

    /* Because a(n) depends only on values with the same parity,
       compute the sequence of the relevant parity. */
    seq_t prev2 = 1;                     /* a(parity_start) */
    idx_t i = (n & 1) ? 3 : 2;           /* start at 2 for even n, 3 for odd n */

    for ( ;
          i <= n;
          i += 2 ) {

        /* Compute a(i) = (i-4)*a(i-2).  Cast i to seq_t first
           to avoid unsigned wrap‑around when i<4. */
        seq_t diff = (seq_t)i - 4;
        seq_t cur  = diff * prev2;
        prev2 = cur;
    }

    return prev2; /* a(n) */
}
