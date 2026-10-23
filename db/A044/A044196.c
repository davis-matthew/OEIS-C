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
 * Helpers:   has_11_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_11_in_base8.h"

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
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
