/*
 * A092150  Partial sums of A092674.
 * https://oeis.org/A092150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} b(k), where b(k) = binomial(k+1,2) -
 *            Sum_{i=1..k-1} b(i)*floor(k/i), b(1) = 0 (b = A092674)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A092150_b = NULL;   /* A092674 values, 1-based */
static seq_t *A092150_s = NULL;   /* partial sums, 1-based */
static size_t A092150_cap = 0;
static size_t A092150_have = 0;   /* highest index computed */

static void A092150_ensure(size_t n)
{
    if (n + 1 <= A092150_cap)
        return;
    size_t nc = A092150_cap ? A092150_cap : 64;
    while (nc < n + 1)
        nc <<= 1;
    seq_t *nb = mem_resize(seq_t, A092150_b, nc);
    seq_t *ns = mem_resize(seq_t, A092150_s, nc);
    if (!nb || !ns)
        abort();
    for (size_t i = A092150_cap; i < nc; ++i) {
        nb[i] = 0;
        ns[i] = 0;
    }
    A092150_b = nb;
    A092150_s = ns;
    A092150_cap = nc;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    size_t N = (size_t)n;
    A092150_ensure(N);
    for (size_t m = A092150_have + 1; m <= N; ++m) {
        seq_t val;
        if (m == 1) {
            val = 0;
        } else {
            seq_t mm = (seq_t)m;
            seq_t sum = 0;
            for (size_t j = 1; j < m; ++j)
                sum += A092150_b[j] * (seq_t)(m / j);
            val = mm * (mm + 1) / 2 - sum;
        }
        A092150_b[m] = val;
        A092150_s[m] = (m == 1 ? (seq_t)0 : A092150_s[m - 1]) + val;
        A092150_have = m;
    }
    return A092150_s[N];
}
