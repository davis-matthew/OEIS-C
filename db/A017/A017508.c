/*
 * A017508  a(n) = (11*n + 9)^12.
 * https://oeis.org/A017508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 * n + 9)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 11*n + 9 */
    seq_t base = (seq_t)11 * n + (seq_t)9;
    /* Exponentiate using binary exponentiation */
    seq_t result = (seq_t)1;
    unsigned exp = 12;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
