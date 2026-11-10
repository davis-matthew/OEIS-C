/*
 * A087824  a(n) = number of times n occurs in A087817.
 * https://oeis.org/A087824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with A087817(k) = n, where A087817(k) =
 *            A087817(A087817(A087817(k-1))) + A087817(k-A087817(k-1)),
 *            A087817(1) = A087817(2) = 1; computed by generating A087817
 *            until its values stay above n and counting
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A087824_v = NULL;   /* A087817 values, index 1..A087824_len */
static long long A087824_len = 0;

static void A087824_extend(long long M)
{
    if (M <= A087824_len) return;
    seq_t *tmp = A087824_v
        ? mem_resize(seq_t, A087824_v, (size_t)(M + 2))
        : mem_new(seq_t, (size_t)(M + 2));
    if (!tmp) abort();
    A087824_v = tmp;
    long long start = A087824_len + 1;
    if (A087824_len < 2) {
        A087824_v[1] = (seq_t)1;
        A087824_v[2] = (seq_t)1;
        start = 3;
    }
    for (long long i = start; i <= M; ++i) {
        long long p = (long long)A087824_v[i - 1];   /* a(i-1)       */
        long long q = (long long)A087824_v[p];       /* a(a(i-1))    */
        long long s = i - p;                         /* i - a(i-1)   */
        A087824_v[i] = A087824_v[q] + A087824_v[s];  /* a(a(a(i-1))) + a(i-a(i-1)) */
    }
    A087824_len = M;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    long long M = A087824_len > 64 ? A087824_len : 64;
    for (;;) {
        A087824_extend(M);
        /* require the last quarter of the prefix to exceed n */
        bool ok = true;
        for (long long i = M - M / 4; i <= M; ++i) {
            if (A087824_v[i] <= (seq_t)n) { ok = false; break; }
        }
        if (ok) {
            seq_t cnt = 0;
            for (long long i = 1; i <= M; ++i)
                if (A087824_v[i] == (seq_t)n) ++cnt;
            return cnt;
        }
        M *= 2;
    }
}
