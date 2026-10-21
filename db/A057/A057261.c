/*
 * A057261  Numbers k such that k | 8^k + 7^k + 6^k + 5^k + 4^k + 3^k.
 * https://oeis.org/A057261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer k such that k | 8^k+7^k+6^k+5^k+4^k+3^k */

#include <stdint.h>
#include <stdbool.h>

/* Integer modular exponentiation: base^exp mod mod.  mod>0. */
static inline idx_t modpow(idx_t base, idx_t exp, idx_t mod) {
    if (mod == 1) return 0;
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (idx_t)(((__int128)result * b) % mod);
        b = (idx_t)(((__int128)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* Test whether k satisfies k | 8^k+7^k+6^k+5^k+4^k+3^k. */
static inline bool is_good_k(idx_t k) {
    if (k == 1) return true;
    if ((k & 1) == 0) return false;     /* even k cannot divide the sum (each term even) */
    idx_t sum = 0;
    for (idx_t base = 3; base <= 8; ++base) {
        sum += modpow(base, k, k);
        sum %= k;
    }
    return sum == 0;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, return 0 as safe */
    idx_t count = 0;
    idx_t k = 1;
    while (true) {
        if (is_good_k(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
