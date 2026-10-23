/*
 * A013851  a(n) = 9^(5*n + 2).
 * https://oeis.org/A013851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(5*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = 9;          /* 9 as seq_t */
    idx_t  exp   = 5 * n + 2;  /* exponent 5*n+2 */
    seq_t result = 1;

    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
