/*
 * A073627  a(1)=a(2)=1; for n > 2, a(n) is the smallest integer such that
 *          a(n) > a(n-1) and a(n)+a(n-1) is prime.
 * https://oeis.org/A073627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, for n>2, a(n) is the smallest integer x>a(n-1)
 *            such that x + a(n-1) is prime
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple static cache and a deterministic
   Miller‑Rabin test for 64‑bit integers (the sequence values stay
   within 64 bits for the requested range).  The code is valid C11
   and C++17 alike. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller‑Rabin primality test for n < 2^64. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        unsigned __int128 x = 1, power = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache for computed terms.  The sequence fits in 64 bits up to
   10,000, so a small fixed-size array suffices. */
#define MAX_CACHE 20000
static seq_t seq_cache[MAX_CACHE];
static size_t cache_len = 0;

/* Compute a(n) on demand, caching earlier terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined but harmless */
    if (n > MAX_CACHE) abort();          /* safety guard */

    /* Extend cache if needed. */
    if ((size_t)n > cache_len) {
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                seq_cache[i-1] = (seq_t)1;
                continue;
            }
            seq_t prev = seq_cache[i-2];          /* a(i‑1) */
            seq_t cand = prev + (seq_t)1;         /* start at a(i‑1)+1 */
            for (;;) {
                seq_t sum = cand + prev;          /* cand + a(i‑1) */
                if (is_prime_uint64((uint64_t)sum)) {
                    seq_cache[i-1] = cand;
                    break;
                }
                ++cand;
            }
        }
        cache_len = (size_t)n;
    }
    return seq_cache[n-1];
}
