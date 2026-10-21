/*
 * A072140  The period length of the 'Reverse and Subtract' trajectory of n is
 *          greater than 1.
 * https://oeis.org/A072140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer whose reverse–subtract
 *            trajectory * has a non‑trivial period (i.e. it never reaches 0).
 *            The trajectory is * defined by repeatedly replacing x by |x -
 *            reverse(x)|, where reverse is * the decimal digit reversal. A
 *            number is accepted if during this process a * non‑ze...
 * Range:     u8 none, u16 n <= 660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

/* Test whether a given candidate leads to a non‑zero cycle. */
static bool qualifies(idx_t cand)
{
    seq_t cur = (seq_t)cand;
    /* Keep a small table of encountered values in this trajectory. */
    seq_t seen[256];
    size_t seen_len = 0;

    while (1) {
        if (cur == 0)   /* reached 0 → reject */
            return false;

        /* Look for a repeat (non‑zero cycle). */
        for (size_t i = 0; i < seen_len; ++i)
            if (seen[i] == cur)
                return true;

        /* Store the current value and advance. */
        if (seen_len < 256)
            seen[seen_len++] = cur;

        seq_t rev = reverse_digits(cur);
        cur = (rev > cur) ? (rev - cur) : (cur - rev);
    }
}

/* Append a value to the static cache, growing it as necessary. */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }
    cache[cache_len++] = val;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_checked;
        if (qualifies(last_checked))
            push_cache((seq_t)last_checked);
    }

    return cache[n - 1];
}
