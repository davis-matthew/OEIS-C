/*
 * A023800  Xenodromes: all digits in base 5 are different.
 * https://oeis.org/A023800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the n-th non‑negative integer whose base‑5 expansion uses
 *            every digit at most once
 * Range:     u8 n <= 93, u16 n <= 261, u32 n <= 261, u64 n <= 261,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. no digit 0–4 repeats in the representation). a(1) = 0 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* caches the finite list of all such numbers; only needed once */
static seq_t  cache[261];
static bool   cache_inited = false;
static size_t cache_len    = 0;

/* build the cache on first use */
static void init_cache(void)
{
    if (cache_inited) return;

    size_t cnt = 0;
    for (uint64_t v = 0; ; ++v) {
        bool ok = true;
        if (v == 0) {
            /* 0 has a single digit 0 – always distinct */
        } else {
            unsigned mask = 0;
            uint64_t t = v;
            while (t) {
                unsigned d = t % 5;
                if (mask & (1U << d)) { ok = false; break; }
                mask |= 1U << d;
                t /= 5;
            }
        }
        if (ok) {
            cache[cnt++] = (seq_t)v;
            if (cnt == 261) break;                 /* 261 total numbers (including 0) */
        }
    }
    cache_len    = cnt;
    cache_inited = true;
}

/* returns the n‑th term (n ≥ 1) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            // undefined; return 0 as a safe value
    init_cache();
    if ((size_t)(n - 1) < cache_len) {
        return cache[n - 1];
    }
    return (seq_t)0;                 /* out of range – return 0 (sequence is finite) */
}
