/*
 * A240831  Sequence U(n) arising from analysis of structure of A240830.
 * https://oeis.org/A240831
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   U(n) = A240830(n - A240830(n-1)), where A240830(n)=1 for n<=7
 *            and A240830(n) = Sum_{i=0..6} A240830(n-i-A240830(n-i-1))
 *            otherwise
 * Range:     u8 n <= 921, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9555 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A240830_buf = NULL;
static size_t A240830_cap = 0;
static idx_t A240830_max = 0;

static void A240830_extend(idx_t n)
{
    if ((size_t)(n + 1) > A240830_cap) {
        size_t nc = A240830_cap ? A240830_cap * 2 : 16;
        while (nc < (size_t)(n + 1)) nc *= 2;
        seq_t *t = mem_resize(seq_t, A240830_buf, nc);
        if (!t) return;
        A240830_buf = t;
        A240830_cap = nc;
    }
    for (idx_t i = A240830_max + 1; i <= n; ++i) {
        if (i <= 7) {
            A240830_buf[i] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t j = 0; j <= 6; ++j) {
                idx_t d = (idx_t)(long long)A240830_buf[i - j - 1];
                idx_t idx = i - j - d;
                sum += A240830_buf[idx];
            }
            A240830_buf[i] = sum;
        }
    }
    if (A240830_max < n) A240830_max = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    A240830_extend(n);
    idx_t d = (idx_t)(long long)A240830_buf[n - 1];
    if (d >= n) return (seq_t)0;
    return A240830_buf[n - d];
}
