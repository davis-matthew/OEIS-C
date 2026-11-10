/*
 * A070881  Least k such that A070868(k) = n.
 * https://oeis.org/A070881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{ k : A070868(k) = n }, found by generating A070868
 *            (A070868(1) = A070868(2) = 1; A070868(k) = A070868(A070868(k-1))
 *            + A070868(k+1-2*A070868(k-1))) and searching it for the first
 *            occurrence of n (A070868 is not monotonic, so the whole prefix
 *            is scanned)
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *A070868_v = NULL;
static idx_t A070868_cap = 0;
static idx_t A070868_cnt = 0;

static int A070868_push(void) {
    idx_t i = A070868_cnt + 1;
    if (i >= A070868_cap) {
        idx_t nc = A070868_cap ? A070868_cap * 2 : 1024;
        idx_t *t = (idx_t *)realloc(A070868_v, (size_t)nc * sizeof(idx_t));
        if (!t) return 0;
        A070868_v = t;
        A070868_cap = nc;
    }
    if (i <= 2) {
        A070868_v[i] = 1;
    } else {
        idx_t prev = A070868_v[i - 1];
        idx_t second = i + 1 - 2 * prev;
        A070868_v[i] = A070868_v[prev] + A070868_v[second];
    }
    A070868_cnt = i;
    return 1;
}

seq_t a(idx_t n) {
    idx_t k;
    if (n == 0) return (seq_t)0;
    /* scan already-generated prefix */
    for (k = 1; k <= A070868_cnt; ++k) {
        if (A070868_v[k] == n) return (seq_t)k;
    }
    /* extend until n first appears */
    for (;;) {
        if (!A070868_push()) return (seq_t)0;
        if (A070868_v[A070868_cnt] == n) return (seq_t)A070868_cnt;
        /* safety bound: a value n cannot first appear beyond ~ 64*n + 64 */
        if (A070868_cnt > 64 * n + 1024) return (seq_t)0;
    }
}
