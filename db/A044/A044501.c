/*
 * A044501  Numbers n such that string 3,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base‑6 representation of n contains the
 *            adjacent digits 3 followed by 0, but n+1 does not
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if base‑6 representation of x contains the substring 30. */
static bool has_30(seq_t x)
{
    if (x == 0) return false;
    int digs[128];
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        digs[len++] = (int)(t % (seq_t)6);
        t /= (seq_t)6;
    }
    /* digs[0] is the least‑significant digit.  Look for a 0 followed by a 3. */
    for (int i = 0; i + 1 < len; ++i)
        if (digs[i] == 0 && digs[i + 1] == 3)
            return true;
    return false;
}

/* a(n) returns the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t found = 0;
    static seq_t next = 0;

    while (found < n) {
        seq_t cur = next++;
        bool cur_ok   = has_30(cur);
        bool next_ok  = has_30(cur + (seq_t)1);
        if (cur_ok && !next_ok) {
            if (found >= cache_size) {
                size_t new_cap = cache_size ? cache_size * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_size = new_cap;
            }
            cache[found++] = cur;
        }
    }
    return cache[n - 1];
}
