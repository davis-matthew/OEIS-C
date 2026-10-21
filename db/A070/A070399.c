/*
 * A070399  a(n) = 6^n mod 31.
 * https://oeis.org/A070399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n (mod 31)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 6^n modulo 31 using binary exponentiation.
       The modulus fits in a 64‑bit integer, so we perform all
       arithmetic in uint64_t before casting back to seq_t. */
    const uint64_t mod = 31;          /* modulus fits comfortably in 64 bits */
    uint64_t result = 1 % mod;        /* initial result = 1 */
    uint64_t base   = 6 % mod;        /* base 6 modulo 31 */
    uint64_t exp    = (uint64_t)n;    /* exponent n (idx_t is unsigned 64‑bit) */

    while (exp > 0) {
        if (exp & 1ULL) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)result;
}
