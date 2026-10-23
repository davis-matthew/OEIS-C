/*
 * A013751  a(n) = 12^(3*n + 2).
 * https://oeis.org/A013751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute exponent 3*n + 2 */
    idx_t exp = n * 3;
    exp += 2;

    /* start with 12^1 and multiply up to the desired exponent */
    seq_t result = 12;
    for (idx_t i = 1; i < exp; ++i) {
        result *= 12;
    }
    return result;
}
