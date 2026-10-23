/*
 * A021437  Decimal expansion of 1/433.
 * https://oeis.org/A021437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 433) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t base = 10;
    seq_t mod = 433;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            r = (r * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (10 * r) / mod;
}
