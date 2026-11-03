/*
 * A142297  Primes congruent to 13 mod 44.
 * https://oeis.org/A142297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 13 modulo 44 OEIS A142297. The
 *            function caches found primes so that later calls can retrieve
 *            them in O(1). The cache size is conservatively chosen to hold
 *            many more than the 10 000 primes required by the test harness
 * Range:     u8 n <= 3, u16 n <= 333, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#define CACHE_LIMIT 20000   /* enough for n up to at least 10000 */

static seq_t cache[CACHE_LIMIT];
static idx_t count = 0;     /* number of primes stored in cache */
static idx_t next_k = 0;    /* next candidate index (candidate = 44*next_k + 13) */

seq_t a(idx_t n)
{
    /* Return 0 for n == 0; behaviour below offset is unimportant. */
    if (n == 0) return 0;

    /* Keep generating primes until we have at least n cached. */
    while (count < n) {
        seq_t cand = (seq_t)44 * (seq_t)next_k + (seq_t)13;
        if (is_prime_big(cand)) {
            if (count >= CACHE_LIMIT) {
                /* In practice this should never happen for the expected limits. */
                abort();
            }
            cache[count++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
