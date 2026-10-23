/*
 * A013872  a(n) = 14^(5*n + 3).
 * https://oeis.org/A013872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 14^(5*n + 3) using binary exponentiation. */
    seq_t result = 1;
    seq_t base   = 14;
    idx_t exp    = 5 * n + 3;

    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return result;
}
