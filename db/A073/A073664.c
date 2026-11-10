/*
 * A073664  Even numbers which retain their relative position in A073661 (are
 *          not disturbed in the rearrangement process).
 * https://oeis.org/A073664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k>=1) with A073661(k) = 2k, where A073661 is the
 *            greedy rearrangement of the even numbers with A073661(1)=2 and
 *            each next term the smallest unused even e such that previous + e
 *            + 1 is prime (search)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static unsigned char *A073664_used = NULL;
static size_t A073664_used_cap = 0;
static uint64_t A073664_prev = 0;
static uint64_t A073664_k = 0;
static uint64_t A073664_low = 1;
static uint64_t *A073664_fix = NULL;
static size_t A073664_fix_len = 0;
static size_t A073664_fix_cap = 0;

static void A073664_ensure_used(size_t idx)
{
    if (A073664_used_cap > idx) return;
    size_t nc = A073664_used_cap ? A073664_used_cap * 2 : 1024;
    while (nc <= idx) nc *= 2;
    A073664_used = (unsigned char *)realloc(A073664_used, nc);
    memset(A073664_used + A073664_used_cap, 0, nc - A073664_used_cap);
    A073664_used_cap = nc;
}

static void A073664_step(void)
{
    uint64_t val;
    if (A073664_k == 0) {
        val = 2;
    } else {
        uint64_t c = A073664_low;
        for (;; c++) {
            A073664_ensure_used((size_t)c);
            if (A073664_used[c]) {
                if (c == A073664_low) A073664_low++;
                continue;
            }
            if (is_prime_u64(A073664_prev + 2 * c + 1)) break;
        }
        val = 2 * c;
    }
    A073664_ensure_used((size_t)(val / 2));
    A073664_used[val / 2] = 1;
    A073664_prev = val;
    A073664_k++;
    if (val == 2 * A073664_k) {
        if (A073664_fix_len == A073664_fix_cap) {
            A073664_fix_cap = A073664_fix_cap ? A073664_fix_cap * 2 : 64;
            A073664_fix = (uint64_t *)realloc(A073664_fix, A073664_fix_cap * sizeof(uint64_t));
        }
        A073664_fix[A073664_fix_len++] = val;
    }
}

seq_t a(idx_t n)
{
    while (A073664_fix_len <= (size_t)n) A073664_step();
    return (seq_t)A073664_fix[n];
}
