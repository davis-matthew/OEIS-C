/*
 * A168326  a(n) = (6*n - 3*(-1)^n - 1)/2.
 * https://oeis.org/A168326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (6*n - 3*(-1)^n - 1)/2
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 this returns the nth term of A168326. */
    if (n == 0u) return 0;          /* undefined below offset, but harmless */

    seq_t nn = (seq_t)n;            /* cast index to seq_t */
    seq_t temp = nn * 6;            /* 6*n */

    /* (-1)^n is 1 for even n, -1 for odd n.
       Add or subtract 3 accordingly. */
    if ((n & 1u) == 0u) {           /* n even */
        temp -= (seq_t)3;
    } else {                        /* n odd  */
        temp += (seq_t)3;
    }

    temp -= (seq_t)1;               /* subtract 1 */
    temp /= (seq_t)2;               /* divide by 2 */

    return temp;
}
