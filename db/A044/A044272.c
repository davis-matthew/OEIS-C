/*
 * A044272  Numbers k such that string 2,3 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A044272:
   k such that the base‑9 representation of k contains the substring "23"
   but the base‑9 representation of k-1 does not.
   The function returns the n-th such k (1‑based indexing). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool contains23(seq_t x) {
    /* Returns true iff base‑9 digits of x contain 2 followed by 3. */
    /* For x < 2*9+3=21, no chance to contain the substring. */
    if (x < /* 21 */ ((seq_t)2 * (seq_t)9 + (seq_t)3)) return false;

    /* Extract base‑9 digits into a small stack array. */
    unsigned char digits[64];  /* 64 digits is plenty for 128‑bit numbers */
    size_t len = 0;
    seq_t tmp = x;
    while (tmp) {
        digits[len++] = (unsigned char)(tmp % (seq_t)9);
        tmp /= (seq_t)9;
    }
    if (len < 2) return false;   /* at least two digits needed */

    /* Scan most‑significant to least‑significant for 2 followed by 3. */
    /* digits[0] is least significant, digits[len-1] most. */
    for (size_t i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 3) return true;
    }
    return false;
}

static seq_t *cache = NULL;      /* array of found terms (1‑based) */
static idx_t cache_len = 0;      /* number of terms cached */
static idx_t cache_cap = 0;      /* capacity of cache array */
static seq_t last_checked = 0;   /* last integer examined */

static void ensure_cache(idx_t needed) {
    if (cache_len >= needed) return;

    /* Grow cache array if necessary */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 32;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed: leave cache as is */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan integers until we have the required number of terms */
    seq_t candidate = last_checked + 1;
    while (cache_len < needed) {
        if (contains23(candidate) && !contains23(candidate - (seq_t)1)) {
            cache[cache_len++] = candidate;
        }
        ++candidate;
    }
    last_checked = candidate - (seq_t)1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for 0, but return 0 safely */
    ensure_cache(n);
    return cache[n - 1];
}
