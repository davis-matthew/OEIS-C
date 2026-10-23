/*
 * A166157  a(n) = (8^n+16*(-9)^n)/17.
 * https://oeis.org/A166157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (8^n + 16*(-9)^n)/17 Recurrence: a(n) = 72*a(n-2) -
 *            a(n-1), a(0)=1, a(1)=-8
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t computed_len = 0;   /* number of values actually computed (0 .. computed_len-1) */

/* Ensure capacity for at least 'len' entries. */
static void ensure_capacity(size_t len) {
    if (len <= cache_capacity) return;
    size_t new_cap = len;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* allocation failure */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) {
        ensure_capacity(1);
        if (computed_len < 1) {
            cache[0] = (seq_t)1;
            computed_len = 1;
        }
        return cache[0];
    }
    if (n == 1) {
        ensure_capacity(2);
        if (computed_len < 2) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)-8;
            computed_len = 2;
        }
        return cache[1];
    }
    /* n >= 2 */
    ensure_capacity((size_t)n + 1);
    if (computed_len < (size_t)n + 1) {
        for (idx_t i = computed_len; i <= n; ++i) {
            cache[i] = (seq_t)72 * cache[i - 2] - cache[i - 1];
        }
        computed_len = n + 1;
    }
    return cache[n];
}
