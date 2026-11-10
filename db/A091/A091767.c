/*
 * A091767  a(n) is the largest number such that all numbers from 1 to a(n)
 *          can be expressed using first n terms of A091766.
 * https://oeis.org/A091767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (least m not representable as a(i) or a(i)+-a(j), i<>j <=
 *            n, of A091766) - 1; computed by incremental bitset search
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A091767_terms = NULL;   /* terms of A091766 */
static seq_t *A091767_res = NULL;     /* res[k] = a(k+1) */
static size_t A091767_cnt = 0, A091767_cap = 0;
static uint8_t *A091767_seen = NULL;
static size_t A091767_seen_sz = 0;
static size_t A091767_miss = 1;

static void A091767_ensure_seen(size_t v)
{
    if (v < A091767_seen_sz)
        return;
    size_t cap = A091767_seen_sz ? A091767_seen_sz : 256;
    while (cap <= v)
        cap <<= 1;
    A091767_seen = (uint8_t *)realloc(A091767_seen, cap);
    memset(A091767_seen + A091767_seen_sz, 0, cap - A091767_seen_sz);
    A091767_seen_sz = cap;
}

static void A091767_add(seq_t t)
{
    if (A091767_cnt >= A091767_cap) {
        size_t cap = A091767_cap ? A091767_cap << 1 : 16;
        A091767_terms = mem_resize(seq_t, A091767_terms, cap);
        A091767_res = mem_resize(seq_t, A091767_res, cap);
        A091767_cap = cap;
    }
    size_t tv = (size_t)t;
    A091767_ensure_seen(2 * tv + 2);
    A091767_seen[tv] = 1;
    for (size_t j = 0; j < A091767_cnt; ++j) {
        size_t s = (size_t)A091767_terms[j];
        A091767_seen[tv + s] = 1;
        if (tv > s)
            A091767_seen[tv - s] = 1;
    }
    A091767_terms[A091767_cnt] = t;
    while (A091767_miss < A091767_seen_sz && A091767_seen[A091767_miss])
        ++A091767_miss;
    A091767_res[A091767_cnt] = (seq_t)(A091767_miss - 1);
    ++A091767_cnt;
}

seq_t a(idx_t n)
{
    if (A091767_cnt == 0) {
        A091767_add((seq_t)1);
        A091767_add((seq_t)2);
        A091767_add((seq_t)6);
    }
    while ((idx_t)A091767_cnt < n) {
        seq_t t = A091767_terms[A091767_cnt - 1] + (seq_t)A091767_miss;
        A091767_add(t);
    }
    return A091767_res[n - 1];
}
