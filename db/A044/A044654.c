/*
 * A044654  Numbers n such that string 2,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑9 representation contains the
 *            substring "24" but whose next integer (n+1) does not contain
 *            that substring. The function generates this sequence on demand
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has24(seq_t x)
{
    if (x == 0) return false;
    uint8_t digits[64];
    size_t len = 0;
    while (x != 0) {
        digits[len++] = (uint8_t)(x % 9);
        x /= 9;
    }
    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] == 2 && digits[i - 2] == 4) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static seq_t  next  = 0;

    if (n == 0) return 0;        /* offset is 1; below it is irrelevant */

    while (size < n) {
        seq_t v = next;
        /* Find the next number that satisfies the definition */
        for (;;) {
            if (has24(v) && !has24(v + 1)) {
                /* Ensure space in the cache */
                if (size + 1 > cap) {
                    if (cap == 0) cap = 16;
                    while (cap < size + 1) cap <<= 1;
                    cache = mem_resize(seq_t, cache, cap);
                }
                cache[size++] = v;
                next = v + 1;          /* next search starts after v */
                break;
            }
            ++v;
        }
    }
    return cache[n - 1];
}
