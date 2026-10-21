/*
 * A089683  a(n) = 3^(4*n).
 * https://oeis.org/A089683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = (seq_t)81;      /* base = 81³? no, 81 = 3^4 */
    while (n > 0) {
        if (n & 1)
            result *= base;    /* multiply by current power of 81 */
        n >>= 1;
        if (n == 0)            /* last loop iteration, no need to square further */
            break;
        base *= base;          /* square to get next power of 81 */
    }
    return result;
}
