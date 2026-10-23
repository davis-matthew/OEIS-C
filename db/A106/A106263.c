/*
 * A106263  Row sums of number triangle A106262.
 * https://oeis.org/A106263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (2^(n-k) mod (k+2))
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast modular exponentiation for 64‑bit integers.
   All arithmetic is performed in 64‑bit unsigned integers, safe
   for moduli up to a few million (the typical range used in this
   sequence). */
static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp != 0ULL) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        exp >>= 1ULL;
        if (exp)
            base = (base * base) % mod;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* Modulus is (k+2), exponent is (n-k). */
        uint64_t mod  = (uint64_t)(k + 2);
        uint64_t exp  = (uint64_t)(n - k);
        uint64_t val  = modpow64(2ULL, exp, mod);
        sum += (seq_t)val;
    }
    return sum;
}
