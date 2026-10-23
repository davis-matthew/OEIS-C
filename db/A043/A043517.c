/*
 * A043517  Numbers having one 7 in base 10.
 * https://oeis.org/A043517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly one decimal digit 7; returns the n‑th such number. */
#include <stdlib.h>
#include "../utils/count7.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    size_t req = (size_t)n;          /* n >= 1 for all valid calls */

    if (req > cache_size) {
        seq_t next = (cache_size == 0) ? 7 : cache[cache_size - 1] + 1;

        while (cache_size < req) {
            if (count7(next) == 1) {
                if (cache_size == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = next;
            }
            ++next;
        }
    }

    return cache[req - 1];
}
