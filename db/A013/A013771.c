/*
 * A013771  a(n) = 22^(3*n + 2).
 * https://oeis.org/A013771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(3*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The exponent is at most 3*n+2 which fits in idx_t for all
       admissible n (n ≤ 10 000 in the tests). */
    idx_t exp = 3 * n + 2;

    seq_t base   = (seq_t)22;
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }

    return result;
}
