/*
 * A098330  Maximum entry in n-th row of A097859.
 * https://oeis.org/A098330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0<=k<=n} T(n,k), where T(m,0)=T(m,m)=2 and T(m,k) =
 *            sopfr(T(m-1,k-1)) + sopfr(T(m-1,k)) (A097859), sopfr = sum of
 *            prime factors with multiplicity
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A098330_sopfr(seq_t v)
{
    seq_t s = 0;
    seq_t t = v;
    seq_t p;
    if (t < 2) return 0;
    for (p = 2; p * p <= t; p = (p == 2) ? 3 : p + 2) {
        while (t % p == 0) {
            s += p;
            t /= p;
        }
    }
    if (t > 1) s += t;
    return s;
}

static seq_t *A098330_row = NULL;
static seq_t *A098330_max = NULL;
static idx_t A098330_cap = 0;
static idx_t A098330_done = 0; /* number of rows computed */

seq_t a(idx_t n)
{
    if (n + 1 > A098330_cap) {
        idx_t nc = n + 1;
        if (nc < 64) nc = 64;
        if (A098330_row == NULL) {
            A098330_row = mem_new(seq_t, (size_t)nc);
            A098330_max = mem_new(seq_t, (size_t)nc);
        } else {
            A098330_row = mem_resize(seq_t, A098330_row, (size_t)nc);
            A098330_max = mem_resize(seq_t, A098330_max, (size_t)nc);
        }
        A098330_cap = nc;
    }
    while (A098330_done <= n) {
        idx_t m = A098330_done;
        idx_t k;
        if (m == 0) {
            A098330_row[0] = 2;
        } else {
            A098330_row[m] = 2;
            for (k = m - 1; k >= 1; --k) {
                A098330_row[k] = A098330_sopfr(A098330_row[k - 1]) + A098330_sopfr(A098330_row[k]);
            }
            A098330_row[0] = 2;
        }
        seq_t mx = A098330_row[0];
        for (k = 1; k <= m; ++k)
            if (A098330_row[k] > mx) mx = A098330_row[k];
        A098330_max[m] = mx;
        A098330_done++;
    }
    return A098330_max[n];
}
