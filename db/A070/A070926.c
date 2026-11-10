/*
 * A070926  Records in the Conway's alimentary function A070871.
 * https://oeis.org/A070926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th record (strict new maximum, in order of index m) of
 *            A070871(m) = A002487(m)*A002487(m+1); computed by a
 *            level-by-level branch-and-bound search of the Calkin-Wilf pair
 *            tree (A002487(m),A002487(m+1)), root (1,1), children (a,a+b) and
 *            (a+b,b)
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A070926_rec = NULL;
static idx_t A070926_cnt = 0;
static idx_t A070926_cap = 0;
static seq_t A070926_R = 0;
static int A070926_level = -1;
static seq_t *A070926_fib = NULL;

static void A070926_push(seq_t v)
{
    if (A070926_cnt >= A070926_cap)
    {
        idx_t nc = A070926_cap ? A070926_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A070926_rec, nc);
        if (!t) return;
        A070926_rec = t;
        A070926_cap = nc;
    }
    A070926_rec[A070926_cnt++] = v;
}

static void A070926_dfs(seq_t a, seq_t b, int d)
{
    if (d == 0)
    {
        seq_t v = a * b;
        if (v > A070926_R)
        {
            A070926_R = v;
            A070926_push(v);
        }
        return;
    }
    seq_t S = (a + b) * A070926_fib[d + 2];
    seq_t x = S / 2;
    seq_t y = S - x;
    if (x <= A070926_R / y) return;   /* x*y <= R : cannot beat the record */
    A070926_dfs(a, a + b, d - 1);
    A070926_dfs(a + b, b, d - 1);
}

static void A070926_next_level(void)
{
    int k = ++A070926_level;
    A070926_fib = mem_new(seq_t, (idx_t)k + 4);
    A070926_fib[1] = 1;
    A070926_fib[2] = 1;
    for (int i = 3; i <= k + 2; ++i) A070926_fib[i] = A070926_fib[i - 1] + A070926_fib[i - 2];
    A070926_dfs((seq_t)1, (seq_t)1, k);
    mem_free(A070926_fib);
    A070926_fib = NULL;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A070926_cnt < n) A070926_next_level();
    return A070926_rec[n - 1];
}
