/*
 * A232397  a(n) = ceiling(sqrt(n^4 + n^3 + n^2 + n + 1))^2 - (n^4 + n^3 + n^2
 *          + n + 1).
 * https://oeis.org/A232397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(sqrt(n^4 + n^3 + n^2 + n + 1))^2 - (n^4 + n^3 +
 *            n^2 + n + 1) For n=0: a(0)=0 For n=1: a(1)=4 For odd n>=3:
 *            a(n)=((n-1)/2)^2 - 1 For even n>=0: a(n)=(5/4)n^2 Complexity:
 *            O(1) per call
 * Range:     u8 n <= 15, u16 n <= 229, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle special cases */
    if (n == 0)
        return (seq_t)0;      /* 5/4 * 0 = 0 also works */
    if (n == 1)
        return (seq_t)4;      /* given in the sequence */

    /* Even indices: a(n) = 5/4 n^2 */
    if ((n & (idx_t)1) == 0)
    {
        seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
        return ((seq_t)5 * nn) / (seq_t)4;
    }

    /* Odd indices >= 3: a(n) = ((n-1)/2)^2 - 1 */
    seq_t k = ((seq_t)n - (seq_t)1) / (seq_t)2;
    return k * k - (seq_t)1;
}
