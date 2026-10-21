/*
 * A203907  Successor function for Conway's PRIMEGAME.
 * https://oeis.org/A203907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203907.
   a(n) = n * num / den where the first fraction num/den in the following
   list (in order) divides n.  The list of fractions is
   17/91, 78/85, 19/51, 23/38, 29/33, 77/29, 95/23, 77/19,
   1/17, 11/13, 13/11, 15/2, 1/7, 55/1.
   Since each fraction is in lowest terms, a fraction num/den works iff
   den divides n, and the value is a(n) = (n/den) * num. */

seq_t a(idx_t n)
{
    static const int numerators[14] = {
        17, 78, 19, 23, 29, 77, 95, 77, 1, 11, 13, 15, 1, 55
    };
    static const int denominators[14] = {
        91, 85, 51, 38, 33, 29, 23, 19, 17, 13, 11, 2, 7, 1
    };

    if (n == 0)
        return 0;                 /* offset starts at 1; 0 is undefined */

    for (int i = 0; i < 14; ++i) {
        int den = denominators[i];
        if ((idx_t)n % (idx_t)den == 0) {
            /* n is divisible by den; compute n/den * num safely. */
            return (seq_t)(n / (idx_t)den) * (seq_t)numerators[i];
        }
    }
    /* All fractions contain 1 as the last denominator, so this line is
       unreachable in practice. */
    return 0;
}
