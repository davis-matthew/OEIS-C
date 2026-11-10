/*
 * A095170  Row sums of A095167.
 * https://oeis.org/A095170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of A095167, the triangle whose n-th row
 *            holds n numbers not coprime to n and not in earlier rows
 *            (computed by direct greedy construction of the triangle)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/mem.h"

static seq_t  *A095167_buf = NULL;
static size_t  A095167_len = 0;
static size_t  A095167_cap = 0;

static uint64_t *A095167_used = NULL;
static size_t    A095167_used_cap = 0;

static uint32_t *A095167_spf = NULL;
static size_t    A095167_spf_cap = 0;

static void A095167_ensure_used(uint64_t x)
{
    size_t idx = (size_t)(x >> 6);
    if (idx >= A095167_used_cap) {
        size_t new_cap = A095167_used_cap ? A095167_used_cap << 1 : 16;
        while (new_cap <= idx) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(A095167_used, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        memset(tmp + A095167_used_cap, 0, (new_cap - A095167_used_cap) * sizeof(uint64_t));
        A095167_used = tmp;
        A095167_used_cap = new_cap;
    }
}

static bool A095167_is_used(uint64_t x)
{
    if (x == 0) return false;
    A095167_ensure_used(x);
    return (A095167_used[x >> 6] >> (x & 63)) & 1ULL;
}

static void A095167_set_used(uint64_t x)
{
    if (x == 0) return;
    A095167_ensure_used(x);
    A095167_used[x >> 6] |= (1ULL << (x & 63));
}

static void A095167_push(seq_t val)
{
    if (A095167_len == A095167_cap) {
        A095167_cap = A095167_cap ? A095167_cap << 1 : 16;
        seq_t *tmp = mem_resize(seq_t, A095167_buf, A095167_cap);
        if (!tmp) abort();
        A095167_buf = tmp;
    }
    A095167_buf[A095167_len++] = val;
}

static void A095167_ensure_spf(size_t limit)
{
    if (A095167_spf_cap > limit) return;
    size_t new_cap = A095167_spf_cap ? A095167_spf_cap << 1 : 64;
    while (new_cap <= limit) new_cap <<= 1;
    uint32_t *tmp = (uint32_t*)realloc(A095167_spf, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    for (size_t i = A095167_spf_cap; i < new_cap; ++i) tmp[i] = (uint32_t)i;
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (tmp[i] == (uint32_t)i) {
            for (size_t j = i * i; j < new_cap; j += i) {
                if (tmp[j] == (uint32_t)j) tmp[j] = (uint32_t)i;
            }
        }
    }
    A095167_spf = tmp;
    A095167_spf_cap = new_cap;
}

static void A095167_generate_row(uint64_t r)
{
    if (r == 1) {
        A095167_push((seq_t)1);
        A095167_set_used(1);
        return;
    }
    A095167_ensure_spf((size_t)r);
    uint32_t primes[16];
    size_t pc = 0;
    uint64_t t = r;
    while (t > 1) {
        uint32_t p = A095167_spf[(size_t)t];
        primes[pc++] = p;
        while (t % p == 0) t /= p;
    }
    uint64_t curr[16];
    for (size_t i = 0; i < pc; ++i) curr[i] = primes[i];
    for (uint64_t cnt = 0; cnt < r; ++cnt) {
        uint64_t min_val = UINT64_MAX;
        size_t min_idx = 0;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t cand = curr[i];
            while (A095167_is_used(cand)) cand += primes[i];
            curr[i] = cand;
            if (cand < min_val) { min_val = cand; min_idx = i; }
        }
        A095167_push((seq_t)min_val);
        A095167_set_used(min_val);
        curr[min_idx] += primes[min_idx];
    }
}

seq_t a(idx_t n)
{
    static uint64_t last_row = 0;
    if (n == 0) return (seq_t)0;
    uint64_t end = (uint64_t)n * ((uint64_t)n + 1) / 2;
    uint64_t start = end - (uint64_t)n;
    while ((uint64_t)A095167_len < end) {
        ++last_row;
        A095167_generate_row(last_row);
    }
    seq_t s = 0;
    for (uint64_t k = start; k < end; ++k) s += A095167_buf[k];
    return s;
}
