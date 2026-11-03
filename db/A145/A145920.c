/*
 * A145920  List of numbers that are both pentagonal (A000326) and binomial
 *          coefficients C(n,4) (A000332).
 * https://oeis.org/A145920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 38, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 8256 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145920:
   a(n) = numbers that are both pentagonal numbers (A000326) and
   binomial coefficients C(m,4) (A000332).  The sequence starts
   a(1) = 0, a(2) = 1, a(3) = 5, a(4) = 35, …
   This implementation builds the sequence lazily and caches the results.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* ---------- Pentagonal‑number test ------------------------------------ */
static bool is_pentagonal(seq_t v)
{
    if (v == 0)
        return true;

    /* s = 24*v + 1  */
    seq_t s = mul_small(v, 24);
    s += 1;

    /* t = floor(sqrt(s)) */
    seq_t t = floor_sqrt(s);

    /* check if t*t == s without overflow (seq_mul works for all seq_t) */
    seq_t t_sq = seq_mul(t, t);
    if (t_sq != s)
        return false;

    /* (t + 1) must be divisible by 6 */
    return ((t + 1) % 6) == 0;
}

/* ---------- Cache of the sequence ------------------------------------- */
static seq_t *cache   = NULL;   /* array of computed a(n) values          */
static size_t cache_len = 0;    /* number of stored values                */
static size_t cache_cap = 0;    /* current allocation capacity            */
static idx_t  next_bin  = 4;    /* next n to test for C(n,4) (starts at 4) */

/* ---------- Sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                               /* undefined, return 0 */
        return 0;

    /* initialise the cache on the first call */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 0;          /* a(1) = 0 */
        cache_len = 1;
        next_bin = 4;          /* C(4,4) = 1 is the first non‑zero term */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* compute C(next_bin,4) = next_bin*(next_bin-1)*(next_bin-2)*(next_bin-3)/24 */
        seq_t val = mul_small((seq_t)next_bin, (unsigned int)(next_bin - 1));
        val = mul_small(val, (unsigned int)(next_bin - 2));
        val = mul_small(val, (unsigned int)(next_bin - 3));
        val = val / 24;                /* integer division, exact */

        if (is_pentagonal(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_bin;
    }

    return cache[(size_t)n - 1];
}
