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
 * Helpers:   has24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has24.h"

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
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
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
