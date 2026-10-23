/*
 * A018837  Number of steps for knight to reach (n,0) on infinite chessboard.
 * https://oeis.org/A018837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(1) = 3 For n ≥ 2: If n is even: a(n) = 2 * ⌊(n +
 *            2)/4⌋ If n is odd : a(n) = 2 * ⌊(n + 1)/4⌋ + 1 (Any n ≥ 0 is
 *            allowed, the result is computed using only integer arithmetic to
 *            avoid overflow.)
 * Range:     u8 n <= 509, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* Compute ⌊(n + c)/4⌋ without overflow: (n/4) + ((n % 4 + c)/4) */
    seq_t part = (seq_t)(n >> 2);          /* n / 4  */
    seq_t rem  = (seq_t)(n & 3);           /* n % 4  */

    if ((n & 1) == 0) {                   /* even n */
        seq_t add = (rem + 2) >> 2;       /* ((n % 4) + 2) / 4  */
        return ((part + add) << 1);       /* 2 * value          */
    } else {                              /* odd n (n != 1)    */
        seq_t add = (rem + 1) >> 2;       /* ((n % 4) + 1) / 4  */
        return (((part + add) << 1) + 1); /* 2 * value + 1     */
    }
}
