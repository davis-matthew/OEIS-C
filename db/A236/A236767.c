/*
 * A236767  Numbers whose square is a fourth power plus a prime.
 * https://oeis.org/A236767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A089001(n)^2 + 1 where A089001(n) is the n-th positive
 *            integer y for which 2*y^2+1 is prime
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- Minimal 64‑bit Miller–Rabin primality test ----------------------------------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        /* modular exponentiation: a^d mod n */
        unsigned __int128 r = 1, p = a % n;
        uint64_t e = d;
        while (e) {
            if (e & 1ULL) r = (r * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        uint64_t x = (uint64_t)r;
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned int rgt = 1; rgt < s; ++rgt) {
            r = (r * r) % n;
            x = (uint64_t)r;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/*--- Cache state --------------------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;   /* number of terms stored, 0 for none */

static uint64_t next_y = 1;   /* next y to test */

/*--- Helper to grow the cache ----------------------------------------------*/
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* out of memory – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/*--- Main sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; return 0 for safety */

    /* compute terms until we reach the requested index */
    while (cache_len < n) {
        /* find next y such that 2*y^2+1 is prime */
        while (true) {
            uint64_t y = next_y;
            uint64_t p = 2ULL * y * y + 1ULL;
            if (is_prime_uint64(p)) {
                /* term = y^2 + 1 */
                seq_t term = (seq_t)(y * y) + (seq_t)1;
                ensure_cache_size(cache_len + 1);
                cache[cache_len++] = term;
                ++next_y;
                break;
            }
            ++next_y;
        }
    }
    return cache[n - 1];
}
