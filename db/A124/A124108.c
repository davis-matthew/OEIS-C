/*
 * A124108  Replace each 1 with 10 in binary representation of n.
 * https://oeis.org/A124108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   replace each 1 with 10 in the binary representation of n
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;

    /* find position of the most significant bit of n */
    idx_t msb = 0;
    idx_t temp = n;
    while (temp >>= 1) {
        ++msb;
    }

    /* process bits from msb down to 0 */
    for (idx_t i = msb; ; --i) {
        if ((n >> i) & 1) {          /* bit is 1 -> append '10' */
            result = (result << 1) | 1; /* append '1'   */
            result = result << 1;        /* append '0'   */
        } else {                      /* bit is 0 -> append '0'   */
            result = result << 1;
        }
        if (i == 0) break;
    }
    return result;
}
