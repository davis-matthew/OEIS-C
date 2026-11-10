/*
 * A075335  Differences of members of pairs in A075333.
 * https://oeis.org/A075335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(n) - r(n), where (r(n), s(n)) is the n-th pair of
 *            A075333 (greedy search: r = smallest unused number, s = smallest
 *            s > r such that r, s, s+r, s-r, r*s are five distinct unused
 *            numbers)
 * Range:     u8 n <= 88, u16 n <= 151, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Validated: standard, pass, 77 terms through n = 150 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A075335_used = NULL;
static size_t A075335_used_cap = 0;
static size_t A075335_used_len = 0;

static uint64_t *A075335_diff = NULL;
static size_t A075335_diff_cap = 0;
static size_t A075335_cnt = 0;

static uint64_t A075335_next_r = 2;

static bool A075335_is_used(uint64_t v)
{
    for (size_t i = 0; i < A075335_used_len; ++i)
        if (A075335_used[i] == v) return true;
    return false;
}

static void A075335_push_used(uint64_t v)
{
    if (A075335_used_len >= A075335_used_cap) {
        A075335_used_cap = A075335_used_cap ? A075335_used_cap * 2 : 64;
        A075335_used = (uint64_t *)realloc(A075335_used, A075335_used_cap * sizeof(uint64_t));
    }
    A075335_used[A075335_used_len++] = v;
}

seq_t a(idx_t n)
{
    while (A075335_cnt < (size_t)n) {
        uint64_t r = A075335_next_r;
        uint64_t s = r + 1;
        for (;;) {
            uint64_t sum = r + s, diff = s - r, prod = r * s;
            if (r == s || r == sum || r == diff || r == prod ||
                s == sum || s == diff || s == prod ||
                sum == diff || sum == prod || diff == prod) {
                ++s;
                continue;
            }
            if (A075335_is_used(r) || A075335_is_used(s) || A075335_is_used(sum) ||
                A075335_is_used(diff) || A075335_is_used(prod)) {
                ++s;
                continue;
            }
            break;
        }
        A075335_push_used(r);
        A075335_push_used(s);
        A075335_push_used(r + s);
        A075335_push_used(s - r);
        A075335_push_used(r * s);

        if (A075335_cnt >= A075335_diff_cap) {
            A075335_diff_cap = A075335_diff_cap ? A075335_diff_cap * 2 : 64;
            A075335_diff = (uint64_t *)realloc(A075335_diff, A075335_diff_cap * sizeof(uint64_t));
        }
        A075335_diff[A075335_cnt++] = s - r;

        uint64_t cand = r + 1;
        while (A075335_is_used(cand)) ++cand;
        A075335_next_r = cand;
    }
    return (seq_t)A075335_diff[(size_t)n - 1];
}
