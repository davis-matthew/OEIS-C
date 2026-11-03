/*
 * A137012  Numbers k such that k and k^2 use only the digits 1, 2, 5 and 9.
 * https://oeis.org/A137012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that k and k^2 use only digits 1, 2, 5 and 9. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Given a non‑negative integer 'curr' whose decimal digits are all in {1,2,5,9},
   return the smallest integer greater than 'curr' that is also in this set.
   For curr==0 the result is 1. */
static seq_t next_allowed(seq_t curr)
{
    if (curr == (seq_t)0)
        return (seq_t)1;

    /* Extract digits least‑significant first. */
    uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = curr;
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    /* Reverse to most‑significant first. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    const uint8_t allowed[4] = {1, 2, 5, 9};

    /* Scan from least‑significant digit to most‑significant. */
    for (int i = (int)len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        bool found = false;
        uint8_t nxt = 0;
        for (int j = 0; j < 4; ++j) {
            if (allowed[j] > d) {
                nxt = allowed[j];
                found = true;
                break;
            }
        }
        if (found) {
            digits[i] = nxt;
            /* Set all less significant digits to the smallest allowed digit (1). */
            for (size_t j = i + 1; j < len; ++j)
                digits[j] = 1;
            /* Convert back to integer. */
            seq_t val = 0;
            for (size_t k = 0; k < len; ++k)
                val = val * (seq_t)10 + (seq_t)digits[k];
            return val;
        }
    }

    /* No higher digit could be incremented – increase length by one with all 1's. */
    seq_t val = 1;
    for (size_t i = 1; i < len + 1; ++i)
        val = val * (seq_t)10 + (seq_t)1;
    return val;
}

/* Return true iff all decimal digits of x are 1, 2, 5, or 9. */
static bool allowed_digits(seq_t x)
{
    /* For zero, treat as allowed (not used in this sequence). */
    if (x == (seq_t)0)
        return true;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d != (seq_t)1 && d != (seq_t)2 && d != (seq_t)5 && d != (seq_t)9)
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0 */

    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static seq_t next_candidate = 0;   /* last processed allowed‑digit number */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_allowed(next_candidate);
        seq_t sq = cand * cand;
        if (allowed_digits(sq)) {
            /* Append cand to cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        /* Prepare for next iteration. */
        next_candidate = cand;
    }

    return cache[n - 1];
}
