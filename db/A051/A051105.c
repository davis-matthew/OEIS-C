/*
 * A051105  First differences of A005229.
 * https://oeis.org/A051105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005229(n+1) - A005229(n), where A005229(1) = A005229(2)
 *            = 1, A005229(n) = A005229(A005229(n-2)) + A005229(n -
 *            A005229(n-2)) for n > 2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 109 terms through n = 4912 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A005229_cache = 0;
static idx_t A005229_len = 0;

static int A005229_fill(idx_t n) {
    if (n >= A005229_len) {
        idx_t new_len = n + 1;
        if (new_len < 2 * A005229_len) new_len = 2 * A005229_len;
        if (new_len < 8) new_len = 8;
        uint64_t *p = (uint64_t *)realloc(A005229_cache, (size_t)new_len * sizeof(uint64_t));
        if (!p) return 0;
        A005229_cache = p;
        idx_t start = A005229_len;
        if (start < 3) {
            A005229_cache[0] = 0;
            A005229_cache[1] = 1;
            A005229_cache[2] = 1;
            start = 3;
        }
        for (idx_t k = start; k < new_len; k++) {
            uint64_t t = A005229_cache[k - 2];
            A005229_cache[k] = A005229_cache[t] + A005229_cache[k - t];
        }
        A005229_len = new_len;
    }
    return 1;
}

seq_t a(idx_t n) {
    if (!A005229_fill(n + 1)) return (seq_t)0;
    return (seq_t)(A005229_cache[n + 1] - A005229_cache[n]);
}
