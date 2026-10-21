/*
 * A089118  Nonnegative numbers in (3*A005836 - 1) [A005836 are the numbers
 *          with base representation containing no 2].
 * https://oeis.org/A089118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3 * (interpret n's binary digits as a base‑3 number) - 1
 * Range:     u8 n <= 19, u16 n <= 639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The index n starts at 1, so a(1) = 2, a(2) = 8, a(3) = 11, ...    */

seq_t a(idx_t n)
{
    seq_t m = 0;           /* value of the A005836 number */
    seq_t pow3 = 1;        /* current power of 3 for the current bit */
    idx_t t = n;           /* iterate over the bits of n */

    while (t)
    {
        if (t & 1)          /* if bit is set, add 3^j to m */
            m += pow3;
        pow3 *= 3;         /* next power of 3 */
        t >>= 1;           /* move to next bit */
    }

    return 3 * m - 1;      /* final result */
}
