/*
 * A105130  Primes of the form 256n+129.
 * https://oeis.org/A105130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime of the form 256k+129
 * (OEIS A105130).  The function returns seq_t,
 * which may be a primitive unsigned integer or an
 * arbitrary‑precision type; the routine works for
 * both builds. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow64.h"

/* ---------- Miller–Rabin for 64‑bit numbers ---------- */

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for offset < 1 */

    uint64_t count = 0;
    uint64_t k = 2;                 /* first arithmetic class element (256*2+129=641) */

    while (true) {
        uint64_t candidate = 256ULL * k + 129ULL;

        /* Avoid overflow: if candidate would wrap, stop */
        if (candidate < 256ULL * k) break;

        if (is_prime_uint64(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++k;
    }

    /* Should not reach here for valid n */
    return 0;
}
