/*
 * A093867  Duplicate of A069549.
 * https://oeis.org/A093867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (p(n+1))^2, where p(k) is the smallest prime ≥ k
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Deterministic Miller-Rabin for 64-bit integers.
   Witness set from https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
*/
static unsigned long long mod_mul64(unsigned long long a, unsigned long long b, unsigned long long mod) {
    return (unsigned __int128)a * b % mod;
}

static unsigned long long mod_pow64(unsigned long long a, unsigned long long e, unsigned long long mod) {
    unsigned long long res = 1;
    while (e) {
        if (e & 1) res = mod_mul64(res, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

static int isPrime64(unsigned long long n) {
    if (n < 2) return 0;
    static const unsigned long long smallPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < (int)(sizeof smallPrimes / sizeof smallPrimes[0]); ++i) {
        if (n == smallPrimes[i]) return 1;
        if (n % smallPrimes[i] == 0) return 0;
    }
    unsigned long long d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const unsigned long long witnesses[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                                  450775ULL, 9780504ULL, 1795265022ULL};
    for (int i = 0; i < (int)(sizeof witnesses / sizeof witnesses[0]); ++i) {
        unsigned long long a = witnesses[i] % n;
        if (a == 0) continue;
        unsigned long long x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int comp = 1;
        for (unsigned long long r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

seq_t a(idx_t n) {
    /* Find smallest prime >= n+1 */
    unsigned long long m = (unsigned long long)n + 1ULL;
    while (!isPrime64(m)) ++m;
    return (seq_t)m * (seq_t)m;
}
