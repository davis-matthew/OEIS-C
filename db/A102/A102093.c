/*
 * A102093  Slowest increasing sequence where the digits, taken one by one,
 *          show the pattern odd/odd/even/even/odd/odd/even/even... Only the
 *          terms < 300 differ from A098161.
 * https://oeis.org/A102093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102093
   Defined as: a(1)=1.
   For n>1, a(n) is the smallest integer >a(n‑1) such that when all terms
   up to a(n) are concatenated, the decimal digits follow the repeating
   pattern odd, odd, even, even, odd, odd, … (period 4).  The pattern
   continues across term boundaries.  The function computes values on demand
   and caches them so that calls can be in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* True if the decimal digits of x, examined from most significant to
   least significant, match the pattern starting at position pat (0‑based).
   *len_out is set to the number of significant digits of x. */
static bool match_and_len(seq_t x, uint8_t pat, int *len_out)
{
    /* We need a dynamic array to hold the digits of x. */
    static int *digits = NULL;
    static size_t cap = 0;
    size_t len = 0;
    seq_t t = x;

    /* Collect digits least significant first. */
    while (t > 0) {
        if (len == cap) {
            cap = cap ? cap * 2 : 16;
            int *tmp = (int *)realloc(digits, cap * sizeof(int));
            if (!tmp) abort();
            digits = tmp;
        }
        digits[len++] = (int)(t % 10);
        t /= 10;
    }
    if (len == 0) {              /* x==0 (should not happen here) */
        *len_out = 0;
        return true;
    }

    /* Test digits from most significant to least. */
    for (size_t i = 0; i < len; ++i) {
        int d = digits[len - 1 - i];
        bool odd_need = ((pat % 4 == 0) || (pat % 4 == 1));
        if (odd_need) {
            if ((d % 2) != 1) return false;
        } else {
            if ((d % 2) != 0) return false;
        }
        pat = (pat + 1) & 3;   /* advance pattern position, modulo 4 */
    }

    *len_out = (int)len;
    return true;
}

/* Computes a(n) for n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;
    static seq_t last_value = 0;
    static uint8_t pattern_pos = 0;   /* next required pattern position */

    /* Return cached value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache until we have at least n terms. */
    while (cache_size < n) {
        /* Expand physical storage if needed. */
        if (cache_size >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* Find the next valid term. */
        seq_t cand = last_value + 1;
        while (true) {
            int len = 0;
            if (match_and_len(cand, pattern_pos, &len)) {
                /* Accept the candidate. */
                cache[cache_size++] = cand;
                last_value = cand;
                pattern_pos = (uint8_t)((pattern_pos + (uint8_t)len) & 3);
                break;
            }
            cand++;
        }
    }

    return cache[n - 1];
}
