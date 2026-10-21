/*
 * A061602  Sum of factorials of the digits of n.
 * https://oeis.org/A061602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of factorials of the decimal digits of n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed 0! to 9! as seq_t */
    static const seq_t fact[10] = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
    };

    /* Special case: n == 0 -> 0! = 1 */
    if (n == 0)
        return fact[0];

    seq_t sum = 0;
    idx_t t = n;
    while (t > 0) {
        /* t%10 returns a idx_t, cast to int to index the array */
        int d = (int)(t % 10);
        sum += fact[d];
        t /= 10;
    }
    return sum;
}
