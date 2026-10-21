/*
 * A061383  Arithmetic mean of digits is an integer.
 * https://oeis.org/A061383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth integer x for which digit_sum(x) % digit_len(x)
 *            == 0
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/ensure_cache.h"

/* Helper: tests whether the arithmetic mean of the decimal digits of x is an integer */
static inline bool mean_is_integer(seq_t x) {
    seq_t sum = digit_sum(x);
    int len = digit_len(x);
    return (len > 0) && ((sum % (seq_t)len) == 0);
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached sequence values   */
    static size_t cap   = 0;      /* current capacity of cache*/
    static idx_t size   = 0;      /* number of values stored   */
    static idx_t cur    = 0;      /* next integer to test      */

    /* Return already computed term if available */
    if (n < size) return cache[n];

    /* Generate terms until we have a(n) */
    while (size <= n) {
        if (mean_is_integer(cur)) {
            if (size >= cap)           /* grow cache if needed   */
                ensure_cache(size, &cache, &cap);
            cache[size++] = cur;       /* store the new term      */
        }
        ++cur;                         /* advance to next integer */
    }
    return cache[n];
}
