/*
 * A050919  a(n) = n*7^n + 1.
 * https://oeis.org/A050919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*7^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binary exponentiation of 7^n */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)7;
    idx_t e = n;
    while (e) {
        if (e & 1)  result *= base;
        base *= base;
        e >>= 1;
    }

    /* Final computation: n * 7^n + 1 */
    seq_t val = (seq_t)n * result + (seq_t)1;
    return val;
}
