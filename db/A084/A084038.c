/*
 * A084038  Primes obtained as the absolute difference of two successive terms
 *          of A084035.
 * https://oeis.org/A084038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |A084035(n+1) - A084035(n)|, where A084035(1) = 1 and
 *            A084035(k) is the smallest unused integer (with the parity of k)
 *            such that the sum and absolute difference with A084035(k-1) are
 *            both prime (computed by a greedy search)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *A084035_seq = NULL;
static size_t A084035_len = 0;
static size_t A084035_cap = 0;
static uint8_t *A084035_used = NULL;
static size_t A084035_used_cap = 0;

static void A084035_ensure_used(uint64_t v)
{
    if (v >= A084035_used_cap) {
        size_t new_cap = A084035_used_cap ? A084035_used_cap * 2 : 128;
        if (new_cap <= v) new_cap = (size_t)v + 1;
        uint8_t *tmp = (uint8_t *)realloc(A084035_used, new_cap);
        if (!tmp) abort();
        memset(tmp + A084035_used_cap, 0, new_cap - A084035_used_cap);
        A084035_used = tmp;
        A084035_used_cap = new_cap;
    }
}

static void A084035_ensure_seq(size_t n)
{
    if (n > A084035_cap) {
        size_t new_cap = A084035_cap ? A084035_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, A084035_seq, new_cap);
        if (!tmp) abort();
        A084035_seq = tmp;
        A084035_cap = new_cap;
    }
}

static seq_t A084035_a(idx_t n)
{
    while (A084035_len < (size_t)n) {
        if (A084035_len == 0) {
            A084035_ensure_seq(1);
            A084035_seq[0] = (seq_t)1;
            A084035_len = 1;
            A084035_ensure_used(1);
            A084035_used[1] = 1;
            continue;
        }
        seq_t prev = A084035_seq[A084035_len - 1];
        uint64_t next_index = (uint64_t)A084035_len + 1;
        uint64_t cand = (next_index % 2 == 0) ? 2ULL : 1ULL;
        uint64_t prev64 = (uint64_t)prev;
        for (;;) {
            A084035_ensure_used(cand);
            if (!A084035_used[(size_t)cand]) {
                uint64_t sum = prev64 + cand;
                uint64_t diff = (prev64 >= cand) ? (prev64 - cand) : (cand - prev64);
                if (is_prime_u64(sum) && is_prime_u64(diff)) {
                    A084035_used[(size_t)cand] = 1;
                    A084035_ensure_seq(A084035_len + 1);
                    A084035_seq[A084035_len] = (seq_t)cand;
                    A084035_len++;
                    break;
                }
            }
            cand += 2ULL;
        }
    }
    return A084035_seq[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t x = A084035_a(n);
    seq_t y = A084035_a(n + 1);
    seq_t xx = A084035_a(n); /* re-fetch: storage may have been reallocated */
    (void)x;
    return (y >= xx) ? (seq_t)(y - xx) : (seq_t)(xx - y);
}
