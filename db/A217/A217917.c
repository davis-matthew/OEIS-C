/*
 * A217917  Odd numbers that appear in sequence A217916.
 * https://oeis.org/A217917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A217916((n+1)*(n+2)/2 - 1), where A217916(m) = prime(m) -
 *            Sum_{k=1..A002024(m)-1} A217916(m-k), A217916(1) = 2
 * Range:     u8 n <= 40, u16 n <= 286, u32 n <= 286, u64 n <= 286,
 *            u128 n <= 286, big any n
 * Validated: standard, pass, 75 terms through n = 275 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint32_t A217916_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
    }

    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }
        if (!ok) {
            cand += 2;
            continue;
        }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

static idx_t A217916_a002024(idx_t m)
{
    idx_t low = 1, high = m;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid * (mid + 1)) / 2 >= m)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t *A217916_cache = NULL;
static size_t A217916_cap = 0;
static size_t A217916_len = 0;

static void A217916_ensure(idx_t n)
{
    if (A217916_len >= n)
        return;

    size_t need = (size_t)n;
    if (A217916_cap < need) {
        size_t new_cap = A217916_cap ? A217916_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *nb = mem_resize(seq_t, A217916_cache, new_cap);
        if (!nb)
            exit(1);
        A217916_cache = nb;
        A217916_cap = new_cap;
    }

    for (idx_t i = (idx_t)A217916_len + 1; i <= n; ++i) {
        if (i == 1) {
            A217916_cache[0] = (seq_t)2;
            continue;
        }
        uint32_t p = A217916_get_prime(i);
        idx_t k = A217916_a002024(i);
        seq_t sum = 0;
        for (idx_t j = 1; j < k; ++j) {
            sum += A217916_cache[i - j - 1];
        }
        A217916_cache[i - 1] = (seq_t)p - sum;
    }
    A217916_len = (size_t)n;
}

static seq_t A217916_a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A217916_ensure(n);
    return A217916_cache[n - 1];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t m = (n + 1) * (n + 2) / 2 - 1;
    return A217916_a(m);
}
