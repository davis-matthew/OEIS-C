/*
 * A138257  Smallest positive integer m such that n divides [4^m/m]
 *          (=A129794(m)).
 * https://oeis.org/A138257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the smallest positive integer m such that
   n divides floor(4^m / m).  The value is computed by testing
   increasing m and checking the divisibility condition. */

#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation:  base^exp mod mod
   (mod must fit in uint64_t).  Uses 128‑bit arithmetic for safety. */
static seq_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) {
            __uint128_t t = ( __uint128_t)result * b;
            result = (uint64_t)(t % mod);
        }
        __uint128_t t = ( __uint128_t)b * b;
        b = (uint64_t)(t % mod);
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    for (idx_t m = 1;; ++m) {
        uint64_t m64 = (uint64_t)m;
        uint64_t mod = n64 * m64;                /* n*m, fits in 64 bits for tested n */
        seq_t pow4 = mod_pow_uint64(4, m, mod);  /* 4^m mod (n*m) */
        seq_t r    = pow4 % (seq_t)m;            /* remainder modulo m */
        seq_t temp = pow4 - r;
        seq_t b    = (temp / (seq_t)m) % (seq_t)n;
        if (b == 0)
            return (seq_t)m;
    }
}
