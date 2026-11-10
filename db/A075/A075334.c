/*
 * A075334  Second members of pairs in A075333.
 * https://oeis.org/A075334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s_n, the second member of the n-th pair (r_n, s_n) in
 *            A075333; greedy search: r = smallest unused number, s = smallest
 *            s>r with r, s, s+r, s-r, r*s five distinct unused numbers
 * Range:     u8 n <= 45, u16 n <= 151, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Validated: standard, pass, 72 terms through n = 146 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A075334_s = NULL;
static size_t A075334_slen = 0, A075334_scap = 0;

static uint64_t *A075334_used = NULL;
static size_t A075334_ulen = 0, A075334_ucap = 0;

static bool A075334_isused(uint64_t v)
{
    for (size_t i = 0; i < A075334_ulen; ++i)
        if (A075334_used[i] == v) return true;
    return false;
}

static void A075334_push_used(uint64_t v)
{
    if (A075334_ulen >= A075334_ucap) {
        A075334_ucap = A075334_ucap ? A075334_ucap * 2 : 64;
        A075334_used = (uint64_t *)realloc(A075334_used, A075334_ucap * sizeof(uint64_t));
    }
    A075334_used[A075334_ulen++] = v;
}

seq_t a(idx_t n)
{
    static uint64_t next_r = 2;
    while (A075334_slen < (size_t)n) {
        uint64_t r = next_r;
        uint64_t s = r + 1;
        for (;;) {
            uint64_t sum = r + s, diff = s - r, prod = r * s;
            if (r == s || r == sum || r == diff || r == prod ||
                s == sum || s == diff || s == prod ||
                sum == diff || sum == prod || diff == prod) { ++s; continue; }
            if (A075334_isused(r) || A075334_isused(s) || A075334_isused(sum) ||
                A075334_isused(diff) || A075334_isused(prod)) { ++s; continue; }
            break;
        }
        A075334_push_used(r);
        A075334_push_used(s);
        A075334_push_used(r + s);
        A075334_push_used(s - r);
        A075334_push_used(r * s);

        if (A075334_slen >= A075334_scap) {
            A075334_scap = A075334_scap ? A075334_scap * 2 : 64;
            A075334_s = (uint64_t *)realloc(A075334_s, A075334_scap * sizeof(uint64_t));
        }
        A075334_s[A075334_slen++] = s;

        uint64_t cand = r + 1;
        while (A075334_isused(cand)) ++cand;
        next_r = cand;
    }
    return (seq_t)A075334_s[(size_t)n - 1];
}
