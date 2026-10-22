/*
 * A005255  Atkinson-Negro-Santoro sequence: a(n+1) = 2*a(n) -
 *          a(n-floor(n/2+1)).
 * https://oeis.org/A005255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = 2*a(n) - a(n - floor(n/2+1)), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n < done) return cache[n];
    if (n >= cap) {
        idx_t newcap = cap ? cap : 16;
        while (newcap <= n) newcap *= 2;
        if (cache == NULL) cache = alloc_zero(newcap);
        else cache = grow_zero(cache, cap, newcap);
        cap = newcap;
    }
    if (done == 0) {
        cache[0] = 0;
        done = 1;
    }
    if (done == 1 && n >= 1) {
        cache[1] = 1;
        done = 2;
    }
    for (idx_t i = done; i <= n; i++) {
        idx_t m = i - 1;
        idx_t j = m - (m / 2 + 1);
        cache[i] = cache[m] * 2 - cache[j];
        done = i + 1;
    }
    return cache[n];
}
