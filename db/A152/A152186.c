/*
 * A152186  a(1)=2; then: if n is even, a(n)=next_square_after a(n - 1), else
 *          a(n)=next_prime_after a(n - 1).
 * https://oeis.org/A152186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1: if n is even, a(n)=next_square_after a(n‑1),
 *            else a(n)=next_prime_after a(n‑1)
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* smallest perfect square strictly greater than x */
static seq_t next_square_after(seq_t x)
{
    seq_t s = floor_sqrt(x);          /* s = ⌊√x⌋  */
    if (s * s <= x)                   /* if x itself is a square or less */
        s = s + (seq_t)1;
    return s * s;
}

/* smallest prime strictly greater than x */
static seq_t next_prime_after(seq_t x)
{
    seq_t cand = x + (seq_t)1;
    if (cand <= (seq_t)2)
        return (seq_t)2;              /* 2 is the first prime */
    if ((cand & (seq_t)1) == (seq_t)0) /* make it odd */
        cand = cand + (seq_t)1;
    while (!is_prime_big(cand))
        cand = cand + (seq_t)2;
    return cand;
}

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t cap      = 0;   /* size of allocation  (≥ last index + 1) */
    static idx_t last     = 0;   /* highest index for which cache is valid */
    static bool   init     = false;

    /* initialization: a(1)=2 */
    if (!init) {
        cap   = 2;                             /* allow indices 0 and 1 */
        cache = mem_resize(seq_t, NULL, cap);
        if (!cache)
            abort();                           /* insufficient memory */
        cache[0] = (seq_t)0;                   /* unused */
        cache[1] = (seq_t)2;
        last   = 1;
        init   = true;
    }

    /* grow the cache if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort();                           /* insufficient memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((i & (idx_t)1) == (idx_t)0)      /* even i → next square */
            cache[i] = next_square_after(prev);
        else                                 /* odd i  → next prime  */
            cache[i] = next_prime_after(prev);
    }
    last = n;
    return cache[n];
}
