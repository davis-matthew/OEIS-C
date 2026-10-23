/*
 * A043554  Essentially same as A005811.
 * https://oeis.org/A043554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 (for n = 0); otherwise a(n) = popcount( n xor (n >> 1)
 *            )
 * Range:     i8 n <= 27, i16 n <= 27, i32 n <= 27, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is a special case */
    if (n == 0) {
        return 1;
    }

    /* Gray code of n */
    idx_t g = n ^ (n >> 1);

    /* Count the number of 1‑bits (population count) */
    unsigned long long cnt = 0;
    while (g) {
        cnt++;
        g &= g - 1;          /* remove the lowest set bit */
    }

    return (seq_t)cnt;
}
