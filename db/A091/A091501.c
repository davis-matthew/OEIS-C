/*
 * A091501  Row sums of triangle A091499, in which A091499(n,k) equals the
 *          k-th term of the convolution of the two prior rows indexed by
 *          (n-k) and (k-1).
 * https://oeis.org/A091501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T(n,0) = T(n,n) = 1 and T(n,k)
 *            = Sum_{j=0..min(n-k,k-1)} T(n-k,j)*T(k-1,k-j-1) for 0<k<n
 *            (A091499)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A091501_tri = NULL;
static seq_t *A091501_sums = NULL;
static size_t A091501_rows = 0;
static size_t A091501_cap_tri = 0;
static size_t A091501_cap_rows = 0;

static void A091501_add_row(void)
{
    size_t r = A091501_rows;
    size_t start = r * (r + 1) / 2;
    size_t need = start + r + 1;
    if (need > A091501_cap_tri) {
        size_t nc = A091501_cap_tri ? A091501_cap_tri * 2 : 256;
        while (nc < need) nc *= 2;
        A091501_tri = mem_resize(seq_t, A091501_tri, nc);
        A091501_cap_tri = nc;
    }
    if (r + 1 > A091501_cap_rows) {
        size_t nc = A091501_cap_rows ? A091501_cap_rows * 2 : 32;
        A091501_sums = mem_resize(seq_t, A091501_sums, nc);
        A091501_cap_rows = nc;
    }
    seq_t total = 0;
    for (size_t k = 0; k <= r; ++k) {
        seq_t val;
        if (k == 0 || k == r) {
            val = 1;
        } else {
            size_t jmax = r - k;
            if (jmax > k - 1) jmax = k - 1;
            size_t s1 = (r - k) * (r - k + 1) / 2;
            size_t s2 = (k - 1) * k / 2;
            val = 0;
            for (size_t j = 0; j <= jmax; ++j)
                val += A091501_tri[s1 + j] * A091501_tri[s2 + (k - j - 1)];
        }
        A091501_tri[start + k] = val;
        total += val;
    }
    A091501_sums[r] = total;
    A091501_rows++;
}

seq_t a(idx_t n)
{
    while (A091501_rows <= (size_t)n) A091501_add_row();
    return A091501_sums[(size_t)n];
}
