/*
 * A215213  Primes congruent to {2, 5, 7} mod 19.
 * https://oeis.org/A215213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 2, 5, or 7 modulo 19, offset
 *            1
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm keeps a cache of already found primes.  When a new n is
   requested we continue generating candidates of the form
   19*k + r with r in {2,5,7}, test them for primality with
   is_prime_big(), and append the ones that succeed to the cache. */

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

/* Determine residue index (0 for 2, 1 for 5, 2 for 7) from a number mod 19. */
static int residue_index_from_mod19(seq_t x) {
    unsigned r = (unsigned)(x % (seq_t)19);
    if (r == 2) return 0;
    if (r == 5) return 1;
    if (r == 7) return 2;
    return -1; /* should never happen */
}

static seq_t *cache      = NULL;  /* array of found primes */
static size_t cache_cap  = 0;      /* current allocation */
static size_t cache_len  = 0;      /* number of primes stored */

static const unsigned incs[3] = {3, 2, 14}; /* 2→5 (+3), 5→7 (+2), 7→2+19 (+14) */

/* Ensure that the cache contains at least n primes. */
static void ensure_cached(idx_t n) {
    if (n <= cache_len) return;           /* already have enough */

    /* Grow the allocation if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    seq_t last_candidate;
    int pre_index;                 /* index into incs for next increment */

    if (cache_len == 0) {
        cache[0] = (seq_t)2U;      /* first prime in the sequence */
        cache_len = 1;
        last_candidate = 2U;
        pre_index = 0;             /* residue index for 2 */
    } else {
        last_candidate = cache[cache_len - 1];
        pre_index = residue_index_from_mod19(last_candidate);
        /* The next candidate will use increments[pre_index]. */
    }

    while (cache_len < n) {
        seq_t candidate = last_candidate + (seq_t)incs[pre_index];
        pre_index = (pre_index + 1) % 3;
        last_candidate = candidate;
        if (is_prime_big(candidate)) {
            cache[cache_len++] = candidate;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* guard for offset 1 */
    ensure_cached(n);
    return cache[n - 1];
}
