/*
 * A006139  n*a(n) = 2*(2*n-1)*a(n-1) + 4*(n-1)*a(n-2) with a(0) = 1.
 * https://oeis.org/A006139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(n,2k) * C(2k,k) * 2^(n-k) for
 *            small n; otherwise n*a(n) = 2*(2*n-1)*a(n-1) + 4*(n-1)*a(n-2),
 *            a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   grow_zero, pw2, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/pw2.h"

static unsigned __int128 small_binom(idx_t m, idx_t k) {
    unsigned __int128 r = 1;
    idx_t i;
    if (k > m) return 0;
    if (k > m - k) k = m - k;
    for (i = 0; i < k; i++)
        r = r * (unsigned __int128)(m - i) / (unsigned __int128)(i + 1);
    return r;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed = 0; /* number of valid entries */

    if (n <= 64) {
        /* every term of the sum is <= a(n), so this is exact whenever a(n) fits */
        seq_t s = 0;
        idx_t k;
        for (k = 0; 2 * k <= n; k++) {
            seq_t c1 = u128_to_seq(small_binom(n, 2 * k));
            seq_t c2 = u128_to_seq(small_binom(2 * k, k));
            s += c1 * c2 * pw2(n - k);
        }
        return s;
    }

    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len : 128;
        while (new_len <= n) new_len *= 2;
        cache = grow_zero(cache, cache_len, new_len);
        cache_len = new_len;
    }
    if (computed == 0) {
        cache[0] = 1;
        cache[1] = 2;
        computed = 2;
    }
    while (computed <= n) {
        idx_t i = computed;
        seq_t num = (seq_t)(4 * i - 2) * cache[i - 1] + (seq_t)(4 * i - 4) * cache[i - 2];
        cache[i] = num / (seq_t)i;
        computed++;
    }
    return cache[n];
}
