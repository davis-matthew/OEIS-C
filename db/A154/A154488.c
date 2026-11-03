/*
 * A154488  a(n+1)-+a(n)=prime, a(1)=1,a(2)=12.
 * https://oeis.org/A154488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=12. For n≥2, a(n+1) is the smallest integer
   > a(n) such that both a(n+1)-a(n) and a(n+1)+a(n) are prime. */
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

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of values stored */

/* Ensure cache has at least 'needed' slots */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* lvalue to produce next term after 'prev' satisfying the primality
   constraints.  The loop terminates because we always find a valid
   candidate (the sequence is defined by existence of such numbers). */
static seq_t next_candidate(seq_t prev)
{
    seq_t cand = prev + (seq_t)1;
    for (;;) {
        seq_t diff = cand - prev;
        seq_t sum  = cand + prev;
        if (is_prime_big(diff) && is_prime_big(sum))
            return cand;
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but safe */

    /* initialise cache with the first two known terms */
    if (cache_len == 0) {
        grow_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache_len = 2;
    }

    /* Return if already computed */
    if ((size_t)(n - 1) < cache_len)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = next_candidate(prev);
        grow_cache(i + 1);
        cache[i] = cand;
    }
    cache_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}
