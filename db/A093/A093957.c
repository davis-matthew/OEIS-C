/*
 * A093957  a(n) = A091799(n) - 3.
 * https://oeis.org/A093957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091799(n) - 3, where A091799(1) = 3 and A091799(n+1) =
 *            max(k,3), with k the maximal number of repeating blocks at the
 *            end of the string A091799(1)A091799(2)...A091799(n). Computed by
 *            direct simulation using the KMP prefix function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9760 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void A091799_append_value(char **buf, size_t *cap, size_t *len, seq_t value)
{
    char tmp[55];
    int pos = 0;
    if (value == 0) {
        tmp[pos++] = '0';
    } else {
        while (value != 0) {
            unsigned d = (unsigned)(value % 10);
            tmp[pos++] = (char)('0' + d);
            value /= 10;
        }
    }
    if (*len + pos > *cap) {
        size_t newcap = (*cap == 0) ? 64 : (*cap << 1);
        while (newcap < *len + pos) newcap <<= 1;
        char *newbuf = (char *)realloc(*buf, newcap);
        if (!newbuf) abort();
        *buf = newbuf;
        *cap = newcap;
    }
    for (int i = pos - 1; i >= 0; --i)
        (*buf)[(*len)++] = tmp[i];
}

static seq_t A091799_compute_k(const char *S, size_t L)
{
    if (L == 0) return 0;
    int *pi = (int *)malloc(L * sizeof(int));
    if (!pi) abort();
    pi[0] = 0;
    for (size_t pos = 1; pos < L; ++pos) {
        int j = pi[pos - 1];
        char cur = S[L - 1 - pos];
        while (j > 0 && cur != S[L - 1 - j]) j = pi[j - 1];
        if (cur == S[L - 1 - j]) ++j;
        pi[pos] = j;
    }
    int max_k = 1;
    for (size_t j = 0; j < L; ++j) {
        int l = (int)(j + 1);
        int period = l - pi[j];
        int k = (l % period == 0) ? (l / period) : 1;
        if (k > max_k) max_k = k;
    }
    free(pi);
    return (seq_t)max_k;
}

static seq_t A091799_a(idx_t n)
{
    if (n == 0) return 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static char *concat = NULL;
    static size_t concat_len = 0;
    static size_t concat_cap = 0;
    static idx_t computed = 0;

    if ((size_t)n > cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 64;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }

    while (computed < n) {
        seq_t val;
        if (computed == 0) {
            val = (seq_t)3;
        } else {
            seq_t k = A091799_compute_k(concat, concat_len);
            val = (k > (seq_t)3) ? k : (seq_t)3;
        }
        cache[computed] = val;
        A091799_append_value(&concat, &concat_cap, &concat_len, val);
        ++computed;
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A091799_a(n) - 3;
}
