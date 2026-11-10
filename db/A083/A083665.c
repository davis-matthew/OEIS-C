/*
 * A083665  Inverse of A083664.
 * https://oeis.org/A083665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A083664(k) = n (inverse permutation);
 *            computed by generating A083664 until the value n appears
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A083664_terms = NULL;     /* 1-based */
static size_t A083664_cap = 0;
static size_t A083664_cnt = 0;
static unsigned char *A083664_used = NULL;
static size_t A083664_used_cap = 0;

static bool A083664_pal(seq_t x)
{
    seq_t rev = 0;
    seq_t t = x;
    while (t != 0) {
        rev = rev * 10 + (t % 10);
        t = t / 10;
    }
    return rev == x;
}

static void A083664_ensure_used(size_t v)
{
    if (v >= A083664_used_cap) {
        size_t nc = v * 2 + 16;
        unsigned char *tmp = (unsigned char *)realloc(A083664_used, nc);
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + A083664_used_cap, 0, nc - A083664_used_cap);
        A083664_used = tmp;
        A083664_used_cap = nc;
    }
}

static void A083664_next(void)
{
    size_t k = A083664_cnt + 1;
    if (k >= A083664_cap) {
        size_t nc = A083664_cap * 2 + 64;
        seq_t *tmp = mem_resize(seq_t, A083664_terms, nc);
        if (!tmp) exit(EXIT_FAILURE);
        A083664_terms = tmp;
        A083664_cap = nc;
    }
    if (k == 1) {
        A083664_terms[1] = 1;
        A083664_ensure_used(1);
        A083664_used[1] = 1;
    } else if (k % 10 == 0) {
        A083664_terms[k] = (seq_t)k;
        A083664_ensure_used(k);
        A083664_used[k] = 1;
    } else {
        size_t d = (k % 10 == 1) ? 2 : 1;
        seq_t target = A083664_terms[k - d];
        size_t m;
        for (m = 1;; ++m) {
            A083664_ensure_used(m);
            if (!A083664_used[m]) {
                if (A083664_pal((seq_t)m * target)) {
                    A083664_terms[k] = (seq_t)m;
                    A083664_used[m] = 1;
                    break;
                }
            }
        }
    }
    A083664_cnt = k;
}

seq_t a(idx_t n)
{
    size_t i;
    seq_t want = (seq_t)n;
    if (n == 0) return 0;
    for (i = 1; i <= A083664_cnt; ++i)
        if (A083664_terms[i] == want) return (seq_t)i;
    for (;;) {
        A083664_next();
        if (A083664_terms[A083664_cnt] == want) return (seq_t)A083664_cnt;
    }
}
