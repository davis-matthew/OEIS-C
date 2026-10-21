/*
 * A070363  a(n) = 3^n mod 46.
 * https://oeis.org/A070363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 46
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 3;
    idx_t exp = n;

    while (exp) {
        if (exp & 1) {
            result = (result * base) % 46;
        }
        base = (base * base) % 46;
        exp >>= 1;
    }
    return result;
}
