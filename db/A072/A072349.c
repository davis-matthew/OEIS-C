/*
 * A072349  a(n) = m such that A064388(m) = n, or 0 if no such m exists.
 * https://oeis.org/A072349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that A064388(m) = n, or 0 if no such m exists.
 *            Computed by generating A064388 (a(1)=1, s=2; subtract s if
 *            result is positive and new, then s=s+1; else add s+i for the
 *            smallest i>=0 giving a new value, then s=s+i+1), recording the
 *            index of each value in a hash table, and stopping at...
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A072349_keys = NULL;
static uint32_t *A072349_vals = NULL;
static size_t A072349_cap = 0;
static size_t A072349_cnt = 0;
static uint64_t A072349_prev = 0;
static uint64_t A072349_s = 0;
static uint64_t A072349_len = 0;

#define A072349_MAXTERMS 2000000ULL

static size_t A072349_slot(uint64_t k, size_t mask)
{
    uint64_t h = k * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)h & mask;
}

static uint32_t A072349_find(uint64_t k)
{
    if (A072349_cap == 0) return 0;
    size_t mask = A072349_cap - 1;
    size_t i = A072349_slot(k, mask);
    while (A072349_keys[i] != 0) {
        if (A072349_keys[i] == k) return A072349_vals[i];
        i = (i + 1) & mask;
    }
    return 0;
}

static void A072349_raw_insert(uint64_t k, uint32_t v)
{
    size_t mask = A072349_cap - 1;
    size_t i = A072349_slot(k, mask);
    while (A072349_keys[i] != 0) i = (i + 1) & mask;
    A072349_keys[i] = k;
    A072349_vals[i] = v;
}

static void A072349_insert(uint64_t k, uint32_t v)
{
    if (A072349_cap == 0 || (A072349_cnt + 1) * 2 > A072349_cap) {
        size_t ncap = A072349_cap ? A072349_cap * 2 : 1024;
        uint64_t *ok = A072349_keys;
        uint32_t *ov = A072349_vals;
        size_t oc = A072349_cap;
        A072349_keys = (uint64_t *)calloc(ncap, sizeof(uint64_t));
        A072349_vals = (uint32_t *)calloc(ncap, sizeof(uint32_t));
        A072349_cap = ncap;
        for (size_t i = 0; i < oc; ++i)
            if (ok[i] != 0) A072349_raw_insert(ok[i], ov[i]);
        free(ok);
        free(ov);
    }
    A072349_raw_insert(k, v);
    ++A072349_cnt;
}

static void A072349_step(void)
{
    if (A072349_len == 0) {
        A072349_prev = 1;
        A072349_s = 2;
        A072349_len = 1;
        A072349_insert(1, 1);
        return;
    }
    uint64_t prev = A072349_prev, s = A072349_s, next;
    if (prev > s && A072349_find(prev - s) == 0) {
        next = prev - s;
        A072349_s = s + 1;
    } else {
        uint64_t i = 0;
        next = prev + s;
        while (A072349_find(next) != 0) {
            ++i;
            next = prev + s + i;
        }
        A072349_s = s + i + 1;
    }
    ++A072349_len;
    A072349_insert(next, (uint32_t)A072349_len);
    A072349_prev = next;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t key = (uint64_t)n;
    uint32_t p = A072349_find(key);
    while (p == 0 && A072349_len < A072349_MAXTERMS) {
        A072349_step();
        p = A072349_find(key);
    }
    return (seq_t)p;
}
