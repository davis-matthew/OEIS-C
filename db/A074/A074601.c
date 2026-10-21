/*
 * A074601  a(n) = 2^n + 6^n.
 * https://oeis.org/A074601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r2 = 1;          /* current 2^k */
    seq_t r6 = 1;          /* current 6^k */
    seq_t b2 = 2;          /* base 2 */
    seq_t b6 = 6;          /* base 6 */
    idx_t e = n;

    while (e) {
        if (e & 1) {          /* if bit is set, multiply result by base */
            r2 *= b2;
            r6 *= b6;
        }
        b2 *= b2;             /* square the base for next bit */
        b6 *= b6;
        e >>= 1;              /* move to the next bit */
    }

    return r2 + r6;
}
