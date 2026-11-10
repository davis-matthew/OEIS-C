/*
 * A075645  Sums of groups in A075643.
 * https://oeis.org/A075645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation of the grouping in A075643. Group n takes, for k =
 *            1..n-1, the smallest unused multiple of k, and then the smallest
 *            unused multiple of n that makes the group sum divisible by n+1.
 *            a(n) = sum of group n (= (n+1)*A075646(n))
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static unsigned char *A075645_used = NULL;
static size_t A075645_ucap = 0;
static seq_t *A075645_memo = NULL;
static idx_t A075645_done = 0;
static idx_t A075645_mcap = 0;

static void A075645_ensure(uint64_t x)
{
    if (x < A075645_ucap) return;
    size_t nc = A075645_ucap ? A075645_ucap : 1024;
    while (nc <= x) nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(A075645_used, nc);
    if (!p) abort();
    for (size_t i = A075645_ucap; i < nc; ++i) p[i] = 0;
    A075645_used = p;
    A075645_ucap = nc;
}

static int A075645_is_used(uint64_t x)
{
    A075645_ensure(x);
    return A075645_used[x];
}

static void A075645_build_group(idx_t n)
{
    uint64_t s = 0;          /* sum mod (n+1) */
    uint64_t mod = (uint64_t)n + 1;
    seq_t total = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint64_t x = k;
        while (A075645_is_used(x)) x += k;
        A075645_used[x] = 1;
        s = (s + x % mod) % mod;
        total += (seq_t)x;
    }
    {
        uint64_t x = n;
        for (;;) {
            if (!A075645_is_used(x) && (s + x % mod) % mod == 0) break;
            x += n;
        }
        A075645_used[x] = 1;
        total += (seq_t)x;
    }
    if (n > A075645_mcap) {
        idx_t nc = A075645_mcap ? A075645_mcap : 64;
        while (nc < n) nc <<= 1;
        if (A075645_memo == NULL)
            A075645_memo = mem_new(seq_t, nc + 1);
        else
            A075645_memo = mem_resize(seq_t, A075645_memo, nc + 1);
        A075645_mcap = nc;
    }
    A075645_memo[n] = total;
}

seq_t a(idx_t n)
{
    while (A075645_done < n) {
        A075645_done++;
        A075645_build_group(A075645_done);
    }
    return A075645_memo[n];
}
