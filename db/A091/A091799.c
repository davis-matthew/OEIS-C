/*
 * A091799  a(1) = 3. To get a(n+1), write the string a(1)a(2)...a(n) as xy^k
 *          for words x and y (where y has positive length) and k is
 *          maximized, i.e., k = the maximal number of repeating blocks at the
 *          end of the sequence so far. Then a(n+1) = max(k,3).
 * https://oeis.org/A091799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = max(k, 3), where k is the maximum number of consecutive *
 *            copies of a word y that appear at the end of the decimal string
 *            * a(1) a(2) ... a(n). a(1) = 3. The function returns a(n) for n
 *            ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9905 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Append the decimal representation of *value* to *buf* (which may grow). */
static void append_value(char **buf, size_t *cap, size_t *len, seq_t value)
{
    char tmp[55];
    int pos = 0;
    if (value == 0) {
        tmp[pos++] = '0';
    } else {
        while (value != 0) {
            unsigned d = (unsigned)(value % 10);
            tmp[pos++] = '0' + d;
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

/* Compute the maximal repetition count k at the end of the string S
 * of length L (S contains decimal digits). */
static seq_t compute_k(const char *S, size_t L)
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
    seq_t max_k = 1;
    for (size_t j = 0; j < L; ++j) {
        int l = (int)(j + 1);
        int period = l - pi[j];
        int k = (l % period == 0) ? (l / period) : 1;
        if ((seq_t)k > max_k) max_k = k;
    }
    free(pi);
    return max_k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 just in case */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static char *concat = NULL;
    static size_t concat_len = 0;
    static size_t concat_cap = 0;
    static idx_t computed = 0;   /* number of terms already computed */

    if (n > cache_cap) {
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
            val = (seq_t)3;          /* a(1) = 3 */
        } else {
            seq_t k = compute_k(concat, concat_len);
            val = (k > (seq_t)3) ? k : (seq_t)3;
        }
        if (computed >= cache_len) cache_len = computed + 1;
        cache[computed] = val;
        append_value(&concat, &concat_cap, &concat_len, val);
        ++computed;
    }
    return cache[n - 1];
}
