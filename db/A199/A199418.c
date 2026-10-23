/*
 * A199418  a(n) = 3*7^n+1.
 * https://oeis.org/A199418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*7^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
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

    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base   = base * base;
        exp >>= 1;
    }

    result = result * 3 + 1;
    return result;
}
