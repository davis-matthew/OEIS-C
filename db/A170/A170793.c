/*
 * A170793  a(n) = n^10*(n + 1)/2.
 * https://oeis.org/A170793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n+1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    if (n % 2 == 0) {                     /* n is even → divide one n by 2   */
        seq_t half = n / 2;
        /* multiply by n nine times */
        for (int i = 0; i < 9; ++i) r *= n;
        r *= half;                        /* incorporate the reduced factor   */
        r *= (n + 1);                     /* last factor (n + 1)              */
    } else {                              /* n is odd → (n+1) is even         */
        seq_t half = (n + 1) / 2;
        /* multiply by n ten times */
        for (int i = 0; i < 10; ++i) r *= n;
        r *= half;                        /* incorporate the reduced factor   */
    }
    return r;
}
