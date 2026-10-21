/*
 * A059469  Numbers that are the products of distinct substrings of
 *          themselves.
 * https://oeis.org/A059469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is produced by brute‑force search: a(n) is the n‑th
   integer that can be expressed as a product of two or more distinct
   decimal substrings of itself. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* Cache of found terms */
static seq_t *terms   = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

/* Next integer to test */
static seq_t next_candidate = 1;

/* Depth‑first search for a product equal to target using distinct substrings */
static bool dfs(const seq_t *sub, size_t k, size_t idx, seq_t prod,
                size_t used, seq_t target)
{
    if (prod == target)          /* reached target */
        return used >= 2;        /* at least two factors required */
    if (idx >= k)                /* no more substrings left */
        return false;

    for (size_t i = idx; i < k; ++i) {
        const seq_t v = sub[i];
        if (v == 0) continue;

        /* Avoid overflow: if prod > target / v then prod*v > target */
        if (v != 0) {
            seq_t max_prod = target / v;
            if (prod > max_prod) continue;
        }

        seq_t next = prod * v;
        if (dfs(sub, k, i + 1, next, used + 1, target))
            return true;
    }
    return false;
}

/* Check whether x satisfies the property */
static bool is_good(seq_t x)
{
    /* Extract decimal digits (least significant first) */
    uint8_t buf[200];
    size_t len;
    get_digits(x, buf, &len);
    if (len < 2) return false;          /* need at least two digits */

    /* Reverse to most‑significant first order */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }

    /* Collect distinct non‑zero substrings that divide x */
    seq_t subs[400];
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t v = 0;
        for (size_t j = i; j < len; ++j) {
            v = v * 10 + buf[j];
            if (v == 0) continue;
            if (v > x) break;
            if (v == x) continue;
            if (x % v != 0) continue;

            /* Ensure uniqueness */
            bool seen = false;
            for (size_t s = 0; s < k; ++s)
                if (subs[s] == v) { seen = true; break; }
            if (!seen) subs[k++] = v;
            if (k == 400) break;     /* safety guard */
        }
        if (k == 400) break;
    }

    if (k < 2) return false;          /* need at least two distinct factors */

    /* Determine if a subset can multiply to x */
    return dfs(subs, k, 0, (seq_t)1, 0, x);
}

/* Sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined below offset */

    /* Generate terms up to the requested index */
    while (term_count < (size_t)n) {
        if (is_good(next_candidate)) {
            /* Grow cache if needed */
            if (term_cap < term_count + 1) {
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                if (new_cap < term_count + 1) new_cap = term_count + 1;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_count++] = next_candidate;
        }
        ++next_candidate;
    }
    return terms[(size_t)n - 1];
}
