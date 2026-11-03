/*
 * A168539  Terms of A123239 which are prime in Z(i), Z(rho) and Z(sqrt(2)).
 * https://oeis.org/A168539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime congruent to 11 (mod 24)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (these are the primes that remain inert in the rings Z(i), Z[ρ] and Z[√2]) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;          /* cached qualifying primes */
static size_t cache_cap = 0;         /* current allocation size   */
static size_t cache_len = 0;         /* number of stored primes    */
static uint64_t last_checked = 1;    /* last candidate examined */

/* enlarge cache until it holds at least n elements */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;          /* nothing to do */

    if (cache_len >= (size_t)n) return;

    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_len < (size_t)n) {
        /* get next candidate congruent to 11 (mod 24) */
        uint64_t cand = last_checked + 1;
        uint64_t rem = cand % 24;
        if (rem <= 11)
            cand += 11 - rem;
        else
            cand += 24 - rem + 11;

        /* test primality */
        if (is_prime_big((seq_t)cand)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len] = (seq_t)cand;
            cache_len++;
        }

        last_checked = cand;     /* continue from here */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */
    ensure_cache(n);
    return cache[n - 1];
}
