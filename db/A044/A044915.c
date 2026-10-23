/*
 * A044915  Numbers whose base-14 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth number whose base‑14 run lengths alternate odd,
 *            even, odd, … starting with odd
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   base14_alternate_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base14_alternate_parity.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Static cache of found terms */
    static seq_t *cache     = NULL;   /* array of sequence values     */
    static size_t size      = 0;      /* number of cached elements    */
    static size_t capacity  = 0;      /* allocated capacity           */
    static seq_t next_candidate = (seq_t)1; /* next integer to test   */

    /* Ensure enough space in cache */
    if (size == capacity) {
        capacity = capacity ? capacity << 1 : 4;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms cached */
    while (size < n) {
        if (base14_alternate_parity(next_candidate)) {
            cache[size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
