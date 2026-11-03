/*
 * A043230  Numbers k such that 1 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the decimal representation
 *            of k contains "17" or "71" but k-1 does not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: check whether x contains the substring "17" or "71" in decimal form. */
static bool has17or71(seq_t x)
{
    if (x < 10) return false;          /* need at least two digits */

    seq_t prev = x % 10;                /* least significant digit */
    x /= 10;
    while (x > 0) {
        seq_t curr = x % 10;
        if ((curr == 1 && prev == 7) || (curr == 7 && prev == 1))
            return true;
        prev = curr;
        x /= 10;
    }
    return false;
}

/* Return the n‑th element of the sequence (n ≥ 1).  The function uses a
   static cache so that repeated calls with arbitrary n are efficient. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;   /* number of cached terms */

    static seq_t next_to_check = 1;

    size_t need = (size_t)n;
    if (sz < need) {
        if (cap < need) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        while (sz < need) {
            seq_t k = next_to_check;
            while (true) {
                if (has17or71(k) && !has17or71(k - 1))
                    break;
                ++k;
            }
            cache[sz++] = k;
            next_to_check = k + 1;
        }
    }

    return cache[need - 1];
}
