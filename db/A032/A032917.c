/*
 * A032917  Numbers having only digits 1 and 3 in their decimal
 *          representation.
 * https://oeis.org/A032917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = replace each binary digit of (n+1) (except the
 *            most‑significant one) by 1 if the digit is 0 and by 3 if the
 *            digit is 1, then interpret the resulting string as a decimal
 *            number
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t m = n + (idx_t)1;           /* use idx_t for bit operations */
    /* find position of the most significant bit in m */
    idx_t pos = 0;
    idx_t temp = m;
    while (temp > (idx_t)1) {
        temp >>= (idx_t)1;
        pos++;
    }
    /* build the decimal number from the remaining bits   */
    seq_t val = 0;
    for (idx_t i = pos; i > 0; --i) {
        idx_t bit = (m >> (i - 1)) & (idx_t)1;
        seq_t digit = (seq_t)(1 + (idx_t)2 * bit);  /* 1→1, 3→3 */
        val = val * (seq_t)10 + digit;
    }
    return val;
}
