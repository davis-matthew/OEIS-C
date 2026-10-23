/*
 * A017208  a(n) = (9*n + 3)^12.
 * https://oeis.org/A017208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 9*n + 3 */
    seq_t base = (seq_t)9 * (seq_t)n + (seq_t)3;

    /* fast exponentiation of base to the 12th power */
    seq_t result = (seq_t)1;
    idx_t exp = (idx_t)12;
    seq_t cur = base;          /* base^(2^k) */

    while (exp > 0) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}
