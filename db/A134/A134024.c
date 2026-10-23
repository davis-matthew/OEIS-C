/*
 * A134024  Number of positive trits in balanced ternary representation of n.
 * https://oeis.org/A134024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   count of positive trits in balanced ternary representation of n
 * Range:     u8 n <= 1314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = n;          /* current value to process */
    seq_t count = 0;        /* number of +1 digits */

    while (cur != 0) {
        seq_t r = cur % 3;  /* remainder modulo 3 */

        if (r == 1) {
            /* digit +1 */
            count++;
            cur = (cur - 1) / 3;
        } else if (r == 2) {
            /* digit -1, carry one to the next ternary place */
            cur = (cur + 1) / 3;
        } else { /* r == 0 */
            cur /= 3;
        }
    }
    return count;
}
