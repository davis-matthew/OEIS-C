/*
 * A034491  a(n) = 7^n + 1.
 * https://oeis.org/A034491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;     /* 7^0 = 1 */
    seq_t base   = (seq_t)7;     /* base = 7 */
    idx_t exp   = n;
    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result + (seq_t)1;    /* add 1 to obtain a(n) */
}
