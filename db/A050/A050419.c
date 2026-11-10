/*
 * A050419  Numbers k such that A050416(k) > A050416(k+1).
 * https://oeis.org/A050419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050416(k) > A050416(k+1); A050416:
 *            b(1)=b(2)=1, b(m+1)=floor(b(m)/3) if not in {0,b(1..m)}, else
 *            b(m)+b(m-1); found by direct generation
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050419_v = NULL;
static size_t A050419_nv = 0, A050419_cap = 0;
static size_t *A050419_d = NULL;
static size_t A050419_nd = 0, A050419_dcap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t target = (size_t)n;

    if (A050419_nv == 0) {
        A050419_cap = 16;
        A050419_v = mem_resize(seq_t, A050419_v, A050419_cap);
        if (!A050419_v) abort();
        A050419_v[1] = 1;
        A050419_v[2] = 1;
        A050419_nv = 2;
    }

    while (A050419_nd < target) {
        if (A050419_cap < A050419_nv + 3) {
            size_t nc = A050419_cap * 2;
            seq_t *p = mem_resize(seq_t, A050419_v, nc);
            if (!p) abort();
            A050419_v = p;
            A050419_cap = nc;
        }
        size_t k = A050419_nv + 1;
        seq_t prev = A050419_v[k - 1];
        seq_t cand = prev / 3;
        bool used = (cand == 0);
        if (!used) {
            for (size_t i = 1; i < k; ++i) {
                if (A050419_v[i] == cand) { used = true; break; }
            }
        }
        if (used) A050419_v[k] = prev + A050419_v[k - 2];
        else A050419_v[k] = cand;
        A050419_nv = k;

        if (A050419_v[k - 1] > A050419_v[k]) {
            if (A050419_dcap <= A050419_nd) {
                size_t nc = A050419_dcap ? A050419_dcap * 2 : 16;
                size_t *p = mem_resize(size_t, A050419_d, nc);
                if (!p) abort();
                A050419_d = p;
                A050419_dcap = nc;
            }
            A050419_d[A050419_nd++] = k - 1;
        }
    }
    return (seq_t)A050419_d[target - 1];
}
