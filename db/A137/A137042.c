/*
 * A137042  Numbers k such that k and k^2 use only the digits 1, 3, 6 and 9.
 * https://oeis.org/A137042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k and k^2 use only the digits 1,3,6,9.
   The function generates candidates in increasing order and returns the
   n‑th one. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* stored sequence terms */
static size_t cache_cap = 0;     /* capacity of cache */
static idx_t cache_len = 0;      /* number of terms stored */

static idx_t last_len = 1;       /* current digit length being enumerated */
static uint64_t offset_in_len = 0; /* offset within [0,4^last_len) */

/* Ensure cache has at least need_cap entries */
static void ensure_cache_cap(size_t need_cap) {
    if (need_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need_cap) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Check whether all decimal digits of x belong to {1,3,6,9}. */
static bool digits_allowed(seq_t x) {
    while (x) {
        int d = (int)(x % 10);
        if (d != 1 && d != 3 && d != 6 && d != 9) return false;
        x /= 10;
    }
    return true;
}

/* Generate the next candidate number composed of digits 1,3,6,9 and
   increment the state variables. Return the candidate number. */
static seq_t next_candidate(void) {
    /* Determine 4^last_len */
    uint64_t pow4 = ((uint64_t)1) << (2 * last_len); /* 4^last_len */
    /* If we finished this length, go to next length */
    if (offset_in_len >= pow4) {
        last_len++;
        offset_in_len = 0;
    }

    /* Build the number from offset_in_len */
    uint64_t t = offset_in_len;
    uint64_t digits[64]; /* supports up to 64 digits */
    const int map[4] = {1, 3, 6, 9};
    for (idx_t i = 0; i < last_len; ++i) {
        digits[i] = map[t & 3];   /* t % 4 */
        t >>= 2;                 /* t /= 4 */
    }
    /* Assemble the number in the correct order (most significant first) */
    seq_t val = 0;
    for (idx_t i = last_len; i > 0; --i) {
        val = val * 10 + digits[i - 1];
    }

    offset_in_len++; /* move to next offset */
    return val;
}

/* Append a value to the cached sequence */
static void append_cache(seq_t val) {
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 0, but safe */
    /* Generate terms until we have at least n terms cached */
    while (cache_len < n) {
        seq_t cand = next_candidate();
        seq_t sq = cand * cand;
        if (digits_allowed(cand) && digits_allowed(sq)) {
            append_cache(cand);
        }
    }
    return cache[n - 1];
}
