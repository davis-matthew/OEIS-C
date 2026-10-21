/*
 * A092977  Duplicate of A057218.
 * https://oeis.org/A092977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = smallest prime of the form n·k! + 1 (k ≥ 1)
 * Range:     i8 n <= 18, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* modular multiplication, avoiding overflow with 128‑bit arithmetic */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation, using binary powering */
static uint64_t mod_pow(uint64_t a, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    while (exp) {
        if (exp & 1ULL) res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* check small primes explicitly to short‑circuit */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 = d · 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* test against strong bases for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL,
        28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n >= 1 by definition of the offset */
    /* compute factorial progressively and test candidates */
    __int128 factorial = 1;   /* 1! = 1 initially */
    for (idx_t k = 1;; ++k) {
        if (k > 1) factorial *= (unsigned long long)k;        /* (k)!   */
        __int128 cand128 = (__int128)n * factorial + 1;       /* n·k!+1 */

        /* the numbers involved are small enough to fit into 64 bits for n up to 1e4 */
        uint64_t cand64 = (uint64_t)cand128;
        if (is_prime_u64(cand64))
            return (seq_t)cand64;
        /* safety guard: avoid infinite loop if factorial overflows 128 bits */
        if (cand128 >= (__int128)UINT64_MAX) break;
    }
    /* In theory this point should never be reached for the problem constraints */
    return (seq_t)-1;
}
