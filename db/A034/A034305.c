/*
 * A034305  Zeroless nonprimes that remain nonprime if any digit is deleted.
 * https://oeis.org/A034305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034305: Zeroless nonprimes that remain nonprime if any digit is deleted.
   Computed by brute‑force search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"

/* Return true iff x is a “zeroless nonprime that remains nonprime after any
   single digit is deleted”. */
static bool is_candidate(seq_t x)
{
    if (x < 10) return false;                 /* need at least two digits */

    /* Extract decimal digits of x in reverse order (least‑significant first). */
    static uint8_t *buf = NULL;
    static size_t buf_cap = 0;
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        if (len >= buf_cap) {
            size_t new_cap = buf_cap ? (buf_cap << 1) : 16;
            buf = (uint8_t*)realloc(buf, new_cap);
            buf_cap = new_cap;
        }
        buf[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* No digit may be zero. */
    for (size_t i = 0; i < len; ++i)
        if (buf[i] == 0) return false;

    /* x must be composite (or 1). */
    if (isPrimeTrial(x)) return false;

    /* Removing any single digit must leave a composite number. */
    for (size_t del = 0; del < len; ++del) {
        seq_t y = 0;
        for (size_t i = 0; i < len; ++i) {
            if (i == del) continue;
            /* Reconstruct in normal order: buf[len-1-i] is the original digit. */
            y = y * 10 + buf[len - 1 - i];
        }
        if (isPrimeTrial(y)) return false;
    }
    return true;
}

/* Grow the static cache of sequence terms. */
static void ensure_cache_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 10;               /* first two‑digit composite */

    while (cache_size < (size_t)n) {
        if (is_candidate(candidate)) {
            ensure_cache_capacity(cache_size + 1, &cache_cap, &cache);
            cache[cache_size++] = candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
