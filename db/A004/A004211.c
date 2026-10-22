/*
 * A004211  Shifts one place left under 2nd-order binomial transform.
 * https://oeis.org/A004211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k)*Stirling2(n, k), via T(n,k) =
 *            2^(n-k)*Stirling2(n,k), T(n,k) = 2*k*T(n-1,k) + T(n-1,k-1),
 *            T(0,0) = 1, a(n) = Sum_k T(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];

    idx_t newlen = n + 1;
    cache = grow_zero(cache, cache_len, newlen);

    seq_t *row = alloc_zero(n + 1);
    row[0] = 1;
    cache[0] = 1;

    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            row[k] = (seq_t)(2 * k) * row[k] + row[k - 1];
        }
        row[0] = 0;
        if (i >= cache_len) {
            seq_t s = 0;
            for (idx_t k = 1; k <= i; k++) s += row[k];
            cache[i] = s;
        }
    }

    release(row, n + 1);
    cache_len = newlen;
    return cache[n];
}
