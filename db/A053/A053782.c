/*
 * A053782  Numbers k such that the sum of the first k composite numbers is
 *          prime.
 * https://oeis.org/A053782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 3291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053782: a(n) = k where the sum of the first k composite numbers is prime */
#include <stdint.h>
#include <stdlib.h>

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static int isPrime64(uint64_t n) {
    if (n < 2) return 0;
    /* quick trial division by small primes */
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return 1;
        if (n % small_primes[i] == 0) return 0;
    }
    /* Miller–Rabin with deterministic bases for 64‑bit */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        int cont = 0;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n-1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* ---------- Global state for memoisation ---------- */
static uint64_t next_comp = 4;          /* next integer to test for composite */
static uint64_t comp_cnt = 0;           /* number of composites processed (k) */
static uint64_t cum_sum = 0;            /* cumulative sum of composites */

static seq_t *cache = NULL;             /* array of k values that give prime sums */
static size_t cache_len = 0;            /* used length */
static size_t cache_cap = 0;            /* allocated capacity */

/* ensure capacity, grow by factor 2 */
static void ensure_cache(void) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    /* n >= 1 expected */
    while (cache_len < n) {
        /* find next composite number */
        while (isPrime64(next_comp)) ++next_comp;

        /* process this composite */
        ++comp_cnt;
        cum_sum += next_comp;
        ++next_comp;                     /* advance for the next step */

        if (isPrime64(cum_sum)) {
            ensure_cache();
            cache[cache_len++] = (seq_t)comp_cnt;
        }
    }
    return cache[n - 1];
}
