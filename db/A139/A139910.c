/*
 * A139910  Primes of the form 7x^2+39y^2.
 * https://oeis.org/A139910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes of the form 7x^2+39y^2 (OEIS A139910)
 * Range:     u8 n <= 4, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached representable primes */
static size_t cache_size = 0;        /* number of primes stored */
static size_t cache_cap  = 0;        /* allocated capacity */
static uint64_t sieve_limit = 20000000ULL; /* initial sieve bound */

/* Residues modulo 1092 for which a prime can be represented. */
static const uint16_t residues[19] = {
    7, 67, 151, 163, 319, 331, 379, 463, 487,
    499, 583, 631, 655, 739, 799, 967, 1003, 1051, 1087
};

/* Quick check if a value is in the residues table. */
static inline bool is_allowed_residue(uint32_t r) {
    for (size_t i = 0; i < sizeof(residues)/sizeof(residues[0]); ++i)
        if (r == residues[i]) return true;
    return false;
}

/* Integer square root (floor) for 64‑bit unsigned values. */
static inline uint64_t floor_sqrt(uint64_t x) {
    if (x < 2ULL) return x;
    uint64_t lo = 0, hi = x, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
            if (mid > hi) break;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Test whether prime p (as uint64_t) can be written as 7x^2+39y^2. */
static bool is_representable(uint64_t p) {
    if (p == 7) return true;
    if ((p % 7) == 0) return false;          /* only 7 itself works */
    uint32_t r = (uint32_t)(p % 1092U);
    if (!is_allowed_residue(r)) return false;

    /* Brute force over y. */
    uint64_t max_y = floor_sqrt(p / 39ULL);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t yy = 39ULL * y * y;
        if (yy > p) break;
        uint64_t rem = p - yy;
        if ((rem % 7ULL) != 0ULL) continue;
        uint64_t sq = rem / 7ULL;
        uint64_t x = floor_sqrt(sq);
        if (x * x == sq) return true;
    }
    return false;
}

/* Build the cache of representable primes up to sieve_limit. */
static void rebuild_cache(void) {
    uint8_t *composite = (uint8_t*)calloc(sieve_limit + 1, 1);
    if (!composite) exit(1);

    /* Sieve of Eratosthenes */
    for (uint64_t i = 2; i * i <= sieve_limit; ++i) {
        if (composite[i]) continue;
        for (uint64_t j = i * i; j <= sieve_limit; j += i)
            composite[j] = 1;
    }

    /* Reallocate cache to hold at least current sieve_limit/50 primes. */
    size_t new_cap = sieve_limit / 50 + 10;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) exit(1);
    cache_cap = new_cap;
    cache_size = 0;

    for (uint64_t i = 2; i <= sieve_limit; ++i) {
        if (composite[i]) continue;
        if (is_representable(i)) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_size++] = (seq_t)i;
        }
    }

    free(composite);
}

/* Ensure the cache has at least n primes.  Grow the sieve bound if needed. */
static void ensure_cache(idx_t n) {
    if (cache_size >= n) return;

    while (cache_size < n) {
        /* Double the sieve limit and rebuild. */
        sieve_limit *= 2;
        rebuild_cache();
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined term, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
