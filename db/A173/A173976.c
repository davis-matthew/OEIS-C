/*
 * A173976  Numbers m such that the concatenation of m and 999 is the lesser
 *          of twin primes, i.e., a millennium twin prime couple.
 * https://oeis.org/A173976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth integer m such that concat(m,999) and *
 *            concat(m,999)+2 are twin primes. Computed by searching
 *            candidates * m ≡ 2 (mod 3)
 * Range:     u8 none, u16 n <= 2, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *cache = NULL;      /* stored values */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Grow cache capacity to at least needed elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th value of A173976 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for index 0, but safe */

    /* Ensure the cache has room for at least n entries. */
    ensure_cache_cap((size_t)n);

    /* static state to keep work across calls */
    static seq_t current_m = 2;      /* starts with the first candidate */
    static bool initialized = false;

    if (!initialized) {
        /* current_m is already set to 2, which is ≡2 (mod 3) */
        initialized = true;
    }

    /* Generate candidates until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Build p = concat(current_m, 999) = current_m * 1000 + 999. */
        seq_t p = current_m * (seq_t)1000 + (seq_t)999;

        /* Check twin prime condition: p and p+2 must be prime. */
        if (is_prime_big(p) && is_prime_big(p + (seq_t)2)) {
            cache[cache_len++] = current_m;
        }

        /* Next candidate must again be ≡ 2 (mod 3). */
        current_m += (seq_t)3;
    }

    return cache[n - 1];
}
