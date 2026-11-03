/*
 * A044096  Numbers n such that string 3,4 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for the n-th integer whose base‑5 representation contains
   the substring “34” but whose predecessor does not. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether a number has the substring 34 in base 5. */
static bool has34(seq_t num)
{
    if (num == 0) return false;
    uint8_t digits[200];
    size_t len = 0;
    seq_t x = num;
    const seq_t five = 5;
    while (x > 0) {
        seq_t rem = x % five;
        digits[len++] = (uint8_t)rem;
        x = x / five;
    }
    /* digits[0] is least significant.  Look for 3 (more significant)
       followed by 4 (less significant). */
    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] == 3 && digits[i - 2] == 4) return true;
    }
    return false;
}

/* Return the n-th element of A044096 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;          /* next candidate to test */

    while (cache_len < (size_t)n) {
        seq_t cand = next;
        bool cand_has = has34(cand);
        seq_t prev = cand - 1;
        bool prev_has = has34(prev);
        if (cand_has && !prev_has) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next;
    }
    return cache[n - 1];
}
