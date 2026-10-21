/*
 * A238720  Number of nX2 0..2 arrays with no element equal to the sum modulo
 *          3 of elements to its left or elements above it.
 * https://oeis.org/A238720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = 2^(2k) a(1) = 2 a(3) = 10 a(odd n>=5) = 5*a(n-2) -
 *            4*a(n-4) (recurrence) (OEIS A238720)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;   /* 1‑based array of computed values   */
static idx_t  max_n   = 0;     /* largest index computed so far      */
static size_t cap      = 0;    /* current allocated capacity         */

static void ensure_capacity(idx_t need) {
    if ((size_t)need + 1 <= cap) return;
    size_t new_cap = cap ? cap * 2 : 32;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();            /* out of memory */
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined; offset is 1 */
    ensure_capacity(n);
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)2;
            } else if (i == 3) {
                cache[i] = (seq_t)10;
            } else if ((i & 1ULL) == 0) {          /* even i */
                cache[i] = pow2(i);
            } else {                            /* odd i >=5 */
                cache[i] = (seq_t)5 * cache[i - 2] - (seq_t)4 * cache[i - 4];
            }
        }
        max_n = n;
    }
    return cache[n];
}
