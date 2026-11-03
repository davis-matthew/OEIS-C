/*
 * A195908  Powers of 7 which have no zero in their decimal expansion.
 * https://oeis.org/A195908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   digit_counts, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195908:
 *   a(n) = 7^k where k is the n-th exponent such that 7^k contains no
 *   decimal digit 0.  The implementation computes successive powers of 7
 *   and keeps those that satisfy the digit condition.  Results are
 *   cached so that any order of calls to a() is fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/digit_counts.h"
#include "../utils/mem.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

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

/* Since the build supplies seq_t and idx_t, we merely rely on them. */
static seq_t *cache      = NULL;  /* array of retained sequence values   */
static size_t cache_len  = 0;      /* number of terms currently cached   */
static size_t cache_cap  = 0;      /* current capacity of cache          */
static seq_t cur          = 0;     /* current power of 7 (7^exponent)    */
static bool   init_done   = false;/* initialization flag                */

seq_t a(idx_t n)
{
    /* Initialise cache and start from the first power of 7 (=1) on the
     * very first call.  Subsequent calls reuse the cached data. */
    if (!init_done) {
        /* Start with a reasonable capacity.  Grow by factor 2 when needed. */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);

        cur = seq_one();                      /* 7^0 = 1 */
        init_done = true;
    }

    /* Generate additional terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        unsigned char digit_cnt[10];
        digit_counts(cur, digit_cnt);

        if (digit_cnt[0] == 0) {              /* no zero digit found       */
            if (cache_len == cache_cap) {     /* need more room */
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len] = cur;           /* store the power          */
            cache_len++;
        }

        /* Advance to the next power of 7. */
        cur = mul_small(cur, 7);
    }

    /* Index in C is zero‑based, while OEIS indexing starts at 1. */
    return cache[(size_t)n - 1];
}
