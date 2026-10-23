/*
 * A179209  Numbers k such that 2^(2*k-1) mod (2*k+1) = 2.
 * https://oeis.org/A179209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 2^(2*k-1) mod (2*k+1) = 2
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* modular exponentiation with 64‑bit arithmetic, safe from overflow */
static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) {
            result = (unsigned __int128)result * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* behavior for n < 1 is unspecified; return 0 */
    if (n == 0) return 0;

    idx_t k = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        uint64_t mod = 2 * k + 1;
        uint64_t exp = 2 * k - 1;
        if (modpow64(2, exp, mod) == 2) {
            cnt++;
            if (cnt == n) return (seq_t)k;
        }
        k++;
    }
    return 0; /* should not reach here */
}
