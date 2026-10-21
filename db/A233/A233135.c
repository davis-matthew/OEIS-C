/*
 * A233135  Shortest (x+1,2x)-code of n.
 * https://oeis.org/A233135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = decimal representation of the shortest (x+1,2x)-code of
 *            n. The code string is built from the binary representation of n:
 *            after the most significant 1 bit, each bit produces a '2'
 *            (doubling step); if the bit is 1 an additional '1' (increment
 *            step) follows. The resulting string of 1's and 2'...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Find the position of the most significant 1 bit (floor(log2(n))). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        msb++;
    }

    /* Build the decimal number consisting of digits 2 (and 1). */
    seq_t result = 0;
    for (idx_t i = msb - 1; ; --i) {
        result = result * 10 + 2;
        if ((n >> i) & 1)
            result = result * 10 + 1;
        if (i == 0)
            break;
    }
    return result;
}
