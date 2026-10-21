/*
 * A216132  11^n mod 10000.
 * https://oeis.org/A216132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n mod 10000
 * Range:     u8 n <= 1, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 11;
    seq_t mod    = 10000;
    while (n > 0) {
        if (n & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
