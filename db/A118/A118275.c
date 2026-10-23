/*
 * A118275  a(0) = 1. a(n) is the number of times the binary representation of
 *          a(n-1) appears in the concatenated string of the terms a(0)
 *          through a(n-1) written in binary. (The concatenated string is
 *          written from left to right and each binary integer is written so
 *          the most significant 1 is on the left.)
 * https://oeis.org/A118275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0)=1; for n>0, a(n)=number of times the binary representation
 *            of a(n-1) occurs in the concatenated binary string of
 *            a(0)..a(n-1), counting overlapping occurrences
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ---------- internal state ---------- */
static seq_t *a_cache = NULL;       /* memoised sequence values             */
static size_t a_cache_len = 0;      /* number of entries stored             */
static size_t a_cache_cap = 0;      /* allocated capacity of a_cache        */

static char *bin_str = NULL;        /* concatenated binary string           */
static size_t bin_len = 0;          /* current length of bin_str            */
static size_t bin_cap = 0;          /* allocated capacity of bin_str        */

/* Ensure capacity of the sequence cache */
static void ensure_seq_cache(size_t needed)
{
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp ? tmp : a_cache;
    a_cache_cap = new_cap;
}

/* Ensure capacity of the binary string buffer */
static void ensure_bin_capacity(size_t needed)
{
    if (needed <= bin_cap)
        return;
    size_t new_cap = bin_cap ? bin_cap << 1 : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    char *tmp = (char *)realloc(bin_str, new_cap);
    bin_str = tmp ? tmp : bin_str;
    bin_cap = new_cap;
}

/* Convert a seq_t value to a binary string stored in buf (no terminator)
   and write the length through len. */
static void seq_to_bin(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    size_t idx = 0;
    seq_t tmp = v;
    while (tmp != 0) {
        buf[idx++] = (tmp & 1) ? '1' : '0';
        tmp >>= 1;
    }
    for (size_t i = 0; i < idx / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[idx - 1 - i];
        buf[idx - 1 - i] = t;
    }
    *len = idx;
}

/* Count overlapping occurrences of pat[0..pat_len-1] in str[0..str_len-1] */
static size_t count_occurrences(const char *str, size_t str_len,
                                const char *pat, size_t pat_len)
{
    if (pat_len == 0 || str_len < pat_len)
        return 0;
    size_t count = 0;
    for (size_t i = 0; i + pat_len <= str_len; ++i) {
        size_t j;
        for (j = 0; j < pat_len; ++j) {
            if (str[i + j] != pat[j])
                break;
        }
        if (j == pat_len)
            ++count;
    }
    return count;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Build sequence entries up to n if necessary */
    if (n >= a_cache_len) {
        ensure_seq_cache((size_t)n + 1);

        for (idx_t i = (idx_t)a_cache_len; i <= n; ++i) {
            if (i == 0) {
                /* a(0) = 1 */
                a_cache[0] = (seq_t)1;
                /* append binary representation of 1 */
                char tmp[128];
                size_t len;
                seq_to_bin(a_cache[0], tmp, &len);
                ensure_bin_capacity(bin_len + len);
                for (size_t k = 0; k < len; ++k)
                    bin_str[bin_len + k] = tmp[k];
                bin_len += len;
            } else {
                /* Build pattern from a(i-1) */
                char pat[256];
                size_t pat_len;
                seq_to_bin(a_cache[i - 1], pat, &pat_len);

                /* Count occurrences in current binary string */
                size_t occ = count_occurrences(bin_str, bin_len, pat, pat_len);
                a_cache[i] = (seq_t)occ;

                /* Append binary of a(i) itself */
                char out[256];
                size_t out_len;
                seq_to_bin(a_cache[i], out, &out_len);
                ensure_bin_capacity(bin_len + out_len);
                for (size_t k = 0; k < out_len; ++k)
                    bin_str[bin_len + k] = out[k];
                bin_len += out_len;
            }
        }
        a_cache_len = (size_t)n + 1;
    }

    return a_cache[(size_t)n];
}
