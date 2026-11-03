/*
 * A044517  Numbers n such that string 5,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for numbers n where the base‑6 representation
 * contains the substring "54" and every such occurrence is followed only by
 * digits 5.  The base‑6 representation of n+1 then contains no "54". */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static bool match54_in_base6(idx_t x)
{
    unsigned char dig[64];
    size_t len = 0;
    idx_t t = x;
    do {
        dig[len++] = (unsigned char)(t % 6);
        t /= 6;
    } while (t);

    bool found = false;
    for (size_t k = 0; k + 1 < len; ++k) {
        if (dig[k] == 4 && dig[k + 1] == 5) {
            if (!found) found = true;
            for (size_t j = 0; j < k; ++j) {
                if (dig[j] != 5) return false;
            }
        }
    }
    return found;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next = 1;

    while (cache_len < n) {
        while (!match54_in_base6(next)) ++next;
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)next);
        ++next;
    }
    return cache[n - 1];
}
