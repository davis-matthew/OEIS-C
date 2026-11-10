/*
 * A073873  Terms of A073872 that do not change their position in the
 *          rearrangement; i.e., values of A073872(n) which equal n(n+1)/2.
 * https://oeis.org/A073873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m(m+1)/2 for the n-th index m such that A073872(m) =
 *            m(m+1)/2 (greedy simulation of A073872, search)
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool *A073873_used = NULL;
static size_t A073873_used_len = 0;
static idx_t *A073873_found = NULL;
static size_t A073873_found_cnt = 0, A073873_found_cap = 0;
static idx_t A073873_m_done = 0;
static idx_t A073873_first_free = 1;

static void A073873_grow_used(size_t needed)
{
    if (A073873_used_len >= needed)
        return;
    size_t nl = A073873_used_len ? A073873_used_len : 1024;
    while (nl < needed)
        nl <<= 1;
    bool *t = (bool *)realloc(A073873_used, nl * sizeof(bool));
    if (!t)
        abort();
    for (size_t i = A073873_used_len; i < nl; ++i)
        t[i] = false;
    A073873_used = t;
    A073873_used_len = nl;
}

static void A073873_step(void)
{
    idx_t m = ++A073873_m_done;
    unsigned __int128 d = (unsigned __int128)2 * m;
    idx_t k = A073873_first_free;
    for (;;)
    {
        if ((size_t)k >= A073873_used_len)
            A073873_grow_used((size_t)k + 1);
        if (!A073873_used[k])
        {
            unsigned __int128 p = (unsigned __int128)k * (k + 1);
            if (p % d == 0)
                break;
        }
        ++k;
    }
    A073873_used[k] = true;
    while ((size_t)A073873_first_free < A073873_used_len && A073873_used[A073873_first_free])
        ++A073873_first_free;
    if (k == m)
    {
        if (A073873_found_cnt >= A073873_found_cap)
        {
            size_t nc = A073873_found_cap ? A073873_found_cap * 2 : 64;
            idx_t *t = (idx_t *)realloc(A073873_found, nc * sizeof(idx_t));
            if (!t)
                abort();
            A073873_found = t;
            A073873_found_cap = nc;
        }
        A073873_found[A073873_found_cnt++] = m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (A073873_found_cnt < (size_t)n)
        A073873_step();
    idx_t m = A073873_found[n - 1];
    return (seq_t)m * ((seq_t)m + 1) / 2;
}
