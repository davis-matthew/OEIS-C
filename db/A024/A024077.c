/*
 * A024077  a(n) = 7^n - n^2.
 * https://oeis.org/A024077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - n^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 7;
    idx_t exp    = n;

    while (exp) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }

    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;
    result -= ns2;
    return result;
}
