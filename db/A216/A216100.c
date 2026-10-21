/*
 * A216100  a(n) = 11^n mod 100.
 * https://oeis.org/A216100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 11^n mod 100
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)100;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)11;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        e >>= 1;
    }
    return result;
}
