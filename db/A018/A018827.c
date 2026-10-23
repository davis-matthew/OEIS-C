/*
 * A018827  Numbers n such that n is a substring of its square in base 3
 *          (written in base 10).
 * https://oeis.org/A018827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer such that its base‑3 representation is a
 *            * substring of the base‑3 representation of its square (computed
 *            by * brute‑force search)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 39, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Convert a seq_t value to base‑3 (most significant digit first).
 * `buf` must have room for at least `cap` characters. Return the number
 * of digits written. */
static size_t to_base3(seq_t v, char *buf, size_t cap)
{
    if (v == (seq_t)0) {
        if (cap == 0) return 0;
        buf[0] = '0';
        return 1;
    }
    size_t len = 0;
    while (len < cap && v != (seq_t)0) {
        buf[len++] = '0' + (int)(v % (seq_t)3);
        v /= (seq_t)3;
    }
    /* reverse to normal order */
    for (size_t i = 0; i + i + 1 < len; ++i) {
        char t = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }
    return len;
}

/* Simple substring test: does pat[0..p_len-1] appear in txt[0..t_len-1]? */
static bool contains_substring(const char *pat, size_t p_len,
                               const char *txt, size_t t_len)
{
    if (p_len == 0 || p_len > t_len) return false;
    for (size_t i = 0; i + p_len <= t_len; ++i) {
        size_t j = 0;
        for (; j < p_len && txt[i + j] == pat[j]; ++j);
        if (j == p_len) return true;
    }
    return false;
}

/* Dynamic cache for already‑found terms. */
static seq_t *cache = NULL;          /* stored terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* current capacity */
static idx_t last_candidate = 0;     /* integer to test next */

/* Append a value to the cache, reallocating if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined but harmless */

    /* Ensure the first term (0) is cached. */
    if (cache_len == 0) {
        cache_append((seq_t)0);
        last_candidate = 0;
    }

    /* Return cached value if we already have it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Buffers for base‑3 representations.  They are large enough for the
       values encountered in typical test ranges. */
    char pat[128];
    char txt[256];
    size_t pat_len, txt_len;

    /* Search for new terms until the nth term is found. */
    while (cache_len < (size_t)n) {
        ++last_candidate;
        seq_t cand = (seq_t)last_candidate;

        pat_len = to_base3(cand, pat, sizeof(pat));
        if (pat_len == 0) pat_len = 1;   /* safety for cand == 0 */

        seq_t sq = cand * cand;
        txt_len = to_base3(sq, txt, sizeof(txt));
        if (txt_len == 0) txt_len = 1;

        if (contains_substring(pat, pat_len, txt, txt_len))
            cache_append(cand);
    }

    return cache[(size_t)n - 1];
}
