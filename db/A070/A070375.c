/*
 * A070375  a(n) = 5^n mod 22.
 * https://oeis.org/A070375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 22
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t MOD  = 22;
    const seq_t BASE = 5;

    seq_t result = 1 % MOD;   /* 5^0 mod 22 */
    seq_t cur    = BASE % MOD;
    idx_t e = n;

    while (e) {
        if (e & 1) {
            result = (result * cur) % MOD;
        }
        cur = (cur * cur) % MOD;
        e >>= 1;
    }
    return result;
}
