/*
 * A070343  a(n) = 3^n mod 25.
 * https://oeis.org/A070343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n % 20;                 /* 3^20 ≡ 1 (mod 25), period = 20 */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    const seq_t mod = (seq_t)25;

    while (k > 0) {
        if (k & 1) {
            result = (result * base) % mod;
        }
        base   = (base * base) % mod;
        k >>= 1;
    }
    return result;
}
