/*
 * A037282  Scan n from left to right once, erasing any primes.
 * https://oeis.org/A037282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Scan n from left to right once, erasing any primes
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ---- small deterministic Miller‑Rabin for 64‑bit integers ---- */
static inline uint64_t _mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    uint64_t res = 0;
    while (b) {
        if (b & 1) res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
#endif
}

static inline uint64_t _mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = _mod_mul(result, base, mod);
        base = _mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool _is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t test_bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(test_bases)/sizeof(test_bases[0]); ++i) {
        uint64_t a = test_bases[i] % n;
        if (a == 0) continue;
        uint64_t x = _mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = _mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* should not occur, offset is 1 */

    /* Convert n to decimal string (array of chars) */
    char buf[22];  /* enough for 20 digits + '\0' */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        buf[len++] = '0' + (tmp % 10);
        tmp /= 10;
    }
    /* reverse to get most‑significant digit first */
    for (int i = 0; i < len / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }

    seq_t result = (seq_t)0;
    int pos = 0;
    while (pos < len) {
        uint64_t val = 0;
        bool found_prime = false;
        int longest = 0;
        for (int j = pos; j < len; ++j) {
            val = val * 10 + (uint64_t)(buf[j] - '0');
            if (_is_prime_uint64(val)) {
                found_prime = true;
                longest = j - pos + 1;
            }
        }
        if (found_prime) {
            pos += longest;     /* erase the prime substring */
        } else {
            result = result * (seq_t)10 + (seq_t)(buf[pos] - '0');
            pos++;
        }
    }
    return result;
}
