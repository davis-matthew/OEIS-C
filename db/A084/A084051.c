/*
 * A084051  Smallest palindrome corresponding to A084050(n).
 * https://oeis.org/A084051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest palindrome obtainable from a permutation of the
 *            digits of A084050(n) (leading zeros dropped); found by search
 *            over A084050, then built from the nonzero digits
 * Range:     u8 n <= 42, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A084051_ok(seq_t n) {
    unsigned int cnt[10] = {0};
    seq_t x = n;
    while (x > (seq_t)0) {
        unsigned int d = (unsigned)(x % (seq_t)10);
        cnt[d]++;
        x = x / (seq_t)10;
    }
    unsigned int odd = 0;
    for (int i = 1; i < 10; ++i)
        if (cnt[i] & 1U) odd++;
    return odd <= 1U;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static seq_t last_checked = (seq_t)0;

    while (cache_size < n) {
        ++last_checked;
        if (A084051_ok(last_checked)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : (size_t)cache_cap * 2;
                seq_t *nb = mem_resize(seq_t, cache, new_cap);
                if (!nb) return (seq_t)0;
                cache = nb;
                cache_cap = (idx_t)new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    seq_t t = cache[n - 1];
    unsigned int cnt[10] = {0};
    while (t > (seq_t)0) {
        unsigned int d = (unsigned)(t % (seq_t)10);
        cnt[d]++;
        t = t / (seq_t)10;
    }

    int mid = -1;
    unsigned int half[10];
    for (int i = 0; i < 10; ++i) {
        half[i] = (i == 0) ? 0U : cnt[i] / 2U;
        if (i > 0 && (cnt[i] & 1U)) mid = i;
    }

    int first = -1;
    for (int i = 1; i < 10; ++i)
        if (half[i] > 0) { first = i; break; }

    if (first < 0) {
        return (seq_t)(mid < 0 ? 0 : mid);
    }

    unsigned char left[256];
    int len = 0;
    left[len++] = (unsigned char)first;
    half[first]--;
    for (int i = 1; i < 10; ++i) {
        for (unsigned int k = 0; k < half[i] && len < 256; ++k)
            left[len++] = (unsigned char)i;
    }

    seq_t v = (seq_t)0;
    for (int i = 0; i < len; ++i)
        v = v * (seq_t)10 + (seq_t)left[i];
    if (mid >= 0)
        v = v * (seq_t)10 + (seq_t)mid;
    for (int i = len - 1; i >= 0; --i)
        v = v * (seq_t)10 + (seq_t)left[i];
    return v;
}
