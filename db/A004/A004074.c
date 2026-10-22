/*
 * A004074  a(n) = 2*A004001(n) - n, where A004001 is the Hofstadter-Conway
 *          $10000 sequence.
 * https://oeis.org/A004074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A004001(n) - n, where A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1)), A004001(1) =
 *            A004001(2) = 1
 * Range:     u8 n <= 2735, u16 n <= 9476, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *h_cache = NULL;
static idx_t h_len = 0;
static idx_t h_cap = 0;

static void ensure_h(idx_t n) {
    if (n < 2) n = 2;
    if (n <= h_len) return;
    if (n > h_cap) {
        idx_t new_cap = (n < 1024) ? 1024 : n + n / 2;
        uint64_t *p = (uint64_t *)realloc(h_cache, (size_t)new_cap * sizeof(uint64_t));
        if (p == NULL) abort();
        h_cache = p;
        h_cap = new_cap;
    }
    if (h_len == 0) {
        h_cache[0] = 1; /* A004001(1) */
        h_cache[1] = 1; /* A004001(2) */
        h_len = 2;
    }
    for (idx_t i = h_len + 1; i <= n; i++) {
        uint64_t prev = h_cache[i - 2];      /* A004001(i-1) */
        uint64_t v = h_cache[prev - 1] + h_cache[(i - prev) - 1];
        h_cache[i - 1] = v;
    }
    if (n > h_len) h_len = n;
}

seq_t a(idx_t n) {
    ensure_h(n);
    seq_t h = (seq_t)h_cache[n - 1];
    return h + h - (seq_t)n;
}
