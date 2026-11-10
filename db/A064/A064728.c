/*
 * A064728  Solutions to A064494(n) = 2(n+1).
 * https://oeis.org/A064728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that A064494(m) = 2*(m+1); found by
 *            increasing search, A064494 computed by reversing the swaps T[k],
 *            k = m..2
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t A064728_val(idx_t n)
{
    idx_t pos = n;
    for (idx_t k = n; k >= 2; --k) {
        if (pos % k == 0) {
            idx_t t = pos / k;
            if (t & 1u) pos += k;
            else pos -= k;
        }
    }
    return pos;
}

static idx_t *A064728_terms = NULL;
static size_t A064728_cnt = 0;
static size_t A064728_cap = 0;
static idx_t A064728_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A064728_cnt < (size_t)n) {
        idx_t m = A064728_next++;
        if (A064728_val(m) == 2 * (m + 1)) {
            if (A064728_cnt == A064728_cap) {
                size_t nc = A064728_cap ? A064728_cap * 2 : 256;
                A064728_terms = (idx_t *)realloc(A064728_terms, nc * sizeof(idx_t));
                A064728_cap = nc;
            }
            A064728_terms[A064728_cnt++] = m;
        }
    }
    return (seq_t)A064728_terms[(size_t)(n - 1)];
}
