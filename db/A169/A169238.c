/*
 * A169238  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   a(n) = 31 * (a(n-1)+...+a(n-i)) + 2  for 1 ≤ n < 28   (i = n)
 *   a(n) = 31 * (a(n-1)+...+a(n-27)) − 496 * a(n-28)  for n ≥ 28
 *
 * This implements the linear recurrence derived from the rational
 * generating function of A169238.  The sequence is memoised in a
 * static cache, so calls may be made in any order.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache   = newp;
        cache_cap = new_cap;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 28) {
            seq_t sum = 0;
            idx_t limit = (i < (idx_t)27) ? i : (idx_t)27;
            for (idx_t j = 1; j <= limit; ++j) {
                sum += cache[i - j] * (seq_t)31;
            }
            seq_t p = (i == 0) ? (seq_t)1 : (seq_t)2;
            cache[i] = sum + p;
        } else {
            seq_t sum = 0;
            for (idx_t j = 1; j <= (idx_t)27; ++j) {
                sum += cache[i - j] * (seq_t)31;
            }
            seq_t term2 = cache[i - 28] * (seq_t)496;
            cache[i] = sum - term2;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
