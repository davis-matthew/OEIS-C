/*
 * A132601  Let S be the concatenation of natural numbers in binary
 *          representation: a(n) = position of first occurrence of n in S,
 *          then this occurrence in S is replaced by a(n) in binary.
 * https://oeis.org/A132601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of first occurrence of the binary representation
 *            of n in the stream S, where S is the concatenation of binary
 *            representations of natural numbers (starting from 1). Each
 *            occurrence is then replaced by the binary representation of
 *            a(n). The implementation builds S incrementally and...
 * Range:     u8 n <= 63, u16 n <= 8193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bin_of_seq_t, bin_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bin_of_seq_t.h"
#include "../utils/bin_of_uint64.h"

/* Memoised sequence values: cache[n] = a(n) for n>=1. */
static seq_t *cache = NULL;          /* 1‑based array */
static size_t cache_cap = 0;         /* allocated size (in seq_t units) */
static idx_t next_compute = 1;       /* next n yet to compute */

/* Binary stream S stored as a character array of '0'/'1'. */
static char *S = NULL;               /* 0‑based */
static size_t S_len = 0;             /* current length */
static size_t S_cap = 0;             /* allocated capacity */

/* Largest natural number whose binary representation has been appended to S. */
static idx_t base_max = 0;

/* ------------------------------------------------------------ */
/* Helper: convert an unsigned 64‑bit value to its binary string.     */

/* ------------------------------------------------------------ */
/* Helper: convert an arbitrary seq_t value to its binary string.   */

/* ------------------------------------------------------------ */
/* Ensure capacity of S array.                                   */
/* ------------------------------------------------------------ */
static void ensure_S_cap(size_t new_cap)
{
    if (new_cap <= S_cap) return;
    size_t cap = S_cap ? S_cap : 64;
    while (cap < new_cap) cap <<= 1;
    char *tmp = (char *)realloc(S, cap);
    if (!tmp) abort();
    S = tmp;
    S_cap = cap;
}

/* ------------------------------------------------------------ */
/* Ensure capacity of cache array.                               */
/* ------------------------------------------------------------ */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 64;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    /* initialise new entries to 0 */
    for (size_t i = cache_cap; i < cap; ++i) cache[i] = 0;
    cache_cap = cap;
}

/* ------------------------------------------------------------ */
/* Append bits from buf[0..len-1] to S.                            */
/* ------------------------------------------------------------ */
static void S_append(const char *buf, size_t len)
{
    ensure_S_cap(S_len + len);
    memcpy(S + S_len, buf, len);
    S_len += len;
}

/* ------------------------------------------------------------ */
/* Find first occurrence of pattern in S; return 0‑based index or SIZE_MAX.   */
/* ------------------------------------------------------------ */
static size_t find_first_occ(const char *pat, size_t pat_len)
{
    if (pat_len == 0 || pat_len > S_len) return SIZE_MAX;
    for (size_t i = 0; i + pat_len <= S_len; ++i) {
        if (S[i] != pat[0]) continue;
        size_t j;
        for (j = 1; j < pat_len; ++j)
            if (S[i + j] != pat[j]) break;
        if (j == pat_len) return i;
    }
    return SIZE_MAX;
}

/* ------------------------------------------------------------ */
/* Replace segment of S beginning at offset, old_len, by new_bits of new_len.  */
/* ------------------------------------------------------------ */
static void S_replace(size_t offset, size_t old_len, const char *new_bits, size_t new_len)
{
    size_t new_total = S_len - old_len + new_len;
    ensure_S_cap(new_total);
    size_t tail_start = offset + old_len;
    size_t tail_len = S_len - tail_start;
    size_t new_tail_start = offset + new_len;
    if (tail_len > 0)
        memmove(S + new_tail_start, S + tail_start, tail_len);
    memcpy(S + offset, new_bits, new_len);
    S_len = new_total;
}

/* ------------------------------------------------------------ */
/* Append binary representations of natural numbers from base_max+1 up to new_base_max. */
/* ------------------------------------------------------------ */
static void extend_base(idx_t new_base_max)
{
    static char buf[65];
    for (idx_t num = base_max + 1; num <= new_base_max; ++num) {
        size_t len = bin_of_uint64((uint64_t)num, buf);
        S_append(buf, len);
    }
    base_max = new_base_max;
}

/* ------------------------------------------------------------ */
/* Main sequence function.                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 */

    /* Ensure cache can hold index n. */
    ensure_cache_cap((size_t)n + 1);

    /* Return memoised value if already computed. */
    if (n < next_compute && cache[n] != 0)
        return cache[n];

    /* Ensure S contains binary representations up to at least n. */
    if (base_max < n)
        extend_base(n);

    /* Compute a(k) for k from next_compute up to n. */
    for (idx_t cur = next_compute; cur <= n; ++cur) {
        /* Binary string of cur. */
        char bin_cur[65];
        size_t len_cur = bin_of_uint64((uint64_t)cur, bin_cur);

        /* Find first occurrence, extending S if necessary. */
        size_t pos_idx = SIZE_MAX;
        while (pos_idx == SIZE_MAX) {
            pos_idx = find_first_occ(bin_cur, len_cur);
            if (pos_idx == SIZE_MAX) {
                /* Need more numbers appended to S. */
                extend_base(base_max + 1);
            }
        }

        /* Position is 1‑based. */
        seq_t pos_val = (seq_t)(pos_idx + 1);
        cache[cur] = pos_val;

        /* Binary representation of the position to replace with. */
        static char bin_pos[128];
        size_t len_pos = bin_of_seq_t(pos_val, bin_pos);

        /* Perform replacement in S. */
        S_replace(pos_idx, len_cur, bin_pos, len_pos);
    }

    /* Update frontier. */
    next_compute = n + 1;

    return cache[n];
}
