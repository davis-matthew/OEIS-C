/*
 * A050090  Numbers k such that b(k) < b(k+1), where b=A050088.
 * https://oeis.org/A050090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with b(k) < b(k+1), where b = A050088 (b(1)=1;
 *            b(k)=floor(b(k-1)/3) if positive and unseen, else 6*b(k-1));
 *            computed by simulating b with a hash set of seen values
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050090_tab = NULL;
static unsigned char *A050090_used = NULL;
static size_t A050090_cap = 0, A050090_cnt = 0;

static size_t A050090_hash(seq_t x, size_t cap)
{
    size_t h = (size_t)(long long)(x % 1000003);
    h = h * (size_t)2654435761u;
    h ^= h >> 15;
    return h & (cap - 1);
}

static bool A050090_has(seq_t x)
{
    size_t i = A050090_hash(x, A050090_cap);
    while (A050090_used[i]) {
        if (A050090_tab[i] == x) return true;
        i = (i + 1) & (A050090_cap - 1);
    }
    return false;
}

static void A050090_raw_insert(seq_t x)
{
    size_t i = A050090_hash(x, A050090_cap);
    while (A050090_used[i]) {
        if (A050090_tab[i] == x) return;
        i = (i + 1) & (A050090_cap - 1);
    }
    A050090_used[i] = 1;
    A050090_tab[i] = x;
    A050090_cnt++;
}

static void A050090_insert(seq_t x)
{
    if ((A050090_cnt + 1) * 2 > A050090_cap) {
        size_t oc = A050090_cap;
        seq_t *ot = A050090_tab;
        unsigned char *ou = A050090_used;
        A050090_cap = oc ? oc * 2 : 1024;
        A050090_tab = mem_new(seq_t, A050090_cap);
        A050090_used = (unsigned char *)calloc(A050090_cap, 1);
        A050090_cnt = 0;
        for (size_t i = 0; i < oc; i++)
            if (ou[i]) A050090_raw_insert(ot[i]);
        if (ot) mem_free(ot);
        if (ou) free(ou);
    }
    A050090_raw_insert(x);
}

seq_t a(idx_t n)
{
    static uint64_t *asc = NULL;
    static size_t nasc = 0, acap = 0;
    static bool inited = false;
    static seq_t cur;
    static uint64_t k;

    if (n == 0) return 0;
    if (!inited) {
        inited = true;
        cur = 1;
        k = 1;
        A050090_insert(cur);
    }
    while (nasc < (size_t)n) {
        seq_t cand = cur / 3;
        seq_t next;
        if (cand > 0 && !A050090_has(cand)) {
            next = cand;
        } else {
            next = cur * 6;
            if (nasc == acap) {
                acap = acap ? acap * 2 : 1024;
                asc = (uint64_t *)realloc(asc, acap * sizeof(uint64_t));
            }
            asc[nasc++] = k;
        }
        A050090_insert(next);
        cur = next;
        k++;
    }
    return (seq_t)asc[(size_t)n - 1];
}
