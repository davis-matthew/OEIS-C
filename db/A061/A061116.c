/*
 * A061116  Numbers coprime to each of their decimal digits.
 * https://oeis.org/A061116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 9701, u32 n <= 9701, u64 n <= 9701,
 *            u128 n <= 9701, big any n
 * Helpers:   digit_counts, gcd, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by checking successive integers and keeping a cache
   of the terms that satisfy the “coprime to each decimal digit” property. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/digit_counts.h"
#include "../utils/grow_seq.h"

/* Cache of generated terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Return true iff x has no digit 0 and is coprime to every digit 2‑9 present. */
static bool has_all_coprime_digits(seq_t x)
{
    if (x == (seq_t)1) {
        /* 1 has only the digit 1 which is always coprime. */
        return true;
    }

    unsigned char cnt[10];
    digit_counts(x, cnt);

    /* any zero digit → fail */
    if (cnt[0]) return false;

    /* digits 2‑9 must be coprime to x */
    for (int d = 2; d < 10; ++d) {
        if (cnt[d]) {
            if (gcd(x, (seq_t)d) != (seq_t)1) return false;
        }
    }
    return true;
}

/* Append the next valid term to the cache. */
static void generate_next(void)
{
    seq_t cand = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (!has_all_coprime_digits(cand)) {
        cand += (seq_t)1;
    }

    if (cache_len == cache_cap) {
        /* ensure capacity for at least one more element */
        grow_seq(&cache, &cache_cap, cache_len + 1);
    }

    cache[cache_len++] = cand;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* n is one–based. Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        generate_next();
    }
    return cache[n - 1];
}
