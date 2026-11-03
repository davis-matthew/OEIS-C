/*
 * A155985  a(1)=1, then a(n) is the smallest square not occurring earlier,
 *          not ending with zero and starting with the last digit of a(n-1).
 * https://oeis.org/A155985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = smallest unused square not ending in 0 and beginning with
 *            the last digit of a(n‑1)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 9620, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9959 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

static seq_t *vec[10] = {0};
static size_t vec_len[10] = {0};
static size_t vec_cap[10] = {0};
static size_t vec_ptr[10] = {0};          /* next unused index in each vec[d] */
static idx_t  global_k = 1;               /* next integer to square */

static void push_vec(int d, seq_t val) {
    if (vec_len[d] >= vec_cap[d]) {
        size_t newcap = vec_cap[d] ? vec_cap[d] * 2 : 16;
        seq_t *newp = mem_resize(seq_t, vec[d], newcap);
        vec[d] = newp;
        vec_cap[d] = newcap;
    }
    vec[d][vec_len[d]++] = val;
}

static void generate_until_digit(int d) {
    /* Ensure vec[d] has an element beyond its current ptr[d]   */
    while (vec_ptr[d] >= vec_len[d]) {
        /* Keep generating until we find a square for digit d   */
        while (1) {
            /* Skip numbers whose square ends with zero          */
            if (global_k % 10 == 0) {
                ++global_k;
                continue;
            }
            seq_t sq = (seq_t)global_k * (seq_t)global_k;
            ++global_k;

            /* Find leading digit via string conversion           */
            char buf[64];
            size_t lenbuf;
            seq_to_str(sq, buf, &lenbuf);
            int lead = buf[0] - '0';

            /* Push square into its corresponding vector          */
            push_vec(lead, sq);

            if (lead == d && vec_len[d] > vec_ptr[d]) {
                break;      /* we have at least one more unused square for d */
            }
        }
    }
}

static seq_t next_square_with_digit(int d) {
    generate_until_digit(d);
    seq_t val = vec[d][vec_ptr[d]++];
    return val;
}

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_size(idx_t n) {
    if (cache_cap < (size_t)n) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < (size_t)n) newcap *= 2;
        seq_t *newp = mem_resize(seq_t, cache, newcap);
        /* Zero out the new area to use 0 as "uncomputed" sentinel */
        memset((char *)newp + cache_cap * sizeof(seq_t),
               0,
               (newcap - cache_cap) * sizeof(seq_t));
        cache = newp;
        cache_cap = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but safe */
    ensure_cache_size(n);
    if (cache[n - 1] != 0) return cache[n - 1];

    /* Build the sequence up to n, if not already present */
    if (cache_len == 0) {                 /* compute the first term */
        cache[0] = 1;
        cache_len = 1;
        /* Mark that the square 1 (leading digit 1) has been used */
        vec_ptr[1] = 1;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];
        int d = (int)(prev % 10);         /* last decimal digit of a(i-1) */
        seq_t val = next_square_with_digit(d);
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
