/*
 * A044196  Numbers n such that string 1,1 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑8 representation contains the
 *            substring "11" and whose predecessor's base‑8 representation
 *            does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if x (in base 8) contains two consecutive 1s. */
static bool has_11_in_base8(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if (d == 1 && prev == 1) return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next_val = 1;

    while (len < n) {
        seq_t cur = next_val;
        bool ok = has_11_in_base8(cur) && !has_11_in_base8(cur - 1);
        if (ok) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cur;
        }
        next_val++;
    }
    return cache[n - 1];
}
