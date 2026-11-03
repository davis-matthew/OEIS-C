/*
 * A192638  Numbers n such that 4n + 3 and 16n + 15 are prime.
 * https://oeis.org/A192638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192638: numbers n such that 4*n+3 and 16*n+15 are prime.
   The function finds the nth term by brute force, caching results for
   random access. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

static inline bool _is_candidate(uint64_t n)
{
    /* any multiple of 3 can never satisfy the condition */
    return (n % 3) != 0;
}

static seq_t *cache = NULL;     /* cached sequence values               */
static size_t cache_len = 0;    /* number of values currently stored     */
static size_t cache_cap = 0;    /* capacity of the array                 */
static uint64_t last_checked = 0; /* last integer examined               */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Generate terms until we have at least 'n' of them. */
    while (cache_len < n) {
        ++last_checked;
        if (!_is_candidate(last_checked))
            continue;

        seq_t cand = (seq_t)last_checked;          /* n itself              */
        seq_t p1   = 4 * cand + (seq_t)3;           /* 4*n + 3               */
        seq_t p2   = 16 * cand + (seq_t)15;          /* 16*n + 15             */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)          /* out of memory – abort gracefully */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;   /* store the found number */
        }
    }

    return cache[n - 1];                 /* a(1) is stored in cache[0] */
}
