/*
 * A092169  a(n) = A000217(n)-A092674(n).
 * https://oeis.org/A092169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(n) - A092674(n), where A000217(n) = n*(n+1)/2 and
 *            A092674(n) = binomial(n+1,2) - Sum_{i=1..n-1}
 *            A092674(i)*floor(n/i), A092674(1) = 0
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A000217_a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (n % 2 == 0)
        return (m / 2) * (m + 1);
    return m * ((m + 1) / 2);
}

static seq_t *A092674_cache = NULL;
static size_t A092674_cap = 0;
static idx_t A092674_max = 0;

static void A092674_ensure(idx_t n)
{
    if ((size_t)n + 1 <= A092674_cap)
        return;
    size_t nc = A092674_cap ? A092674_cap : 64;
    while ((size_t)n + 1 > nc)
        nc <<= 1;
    seq_t *p = mem_resize(seq_t, A092674_cache, nc);
    if (!p)
        abort();
    for (size_t i = A092674_cap; i < nc; ++i)
        p[i] = 0;
    A092674_cache = p;
    A092674_cap = nc;
}

static seq_t A092674_a(idx_t n)
{
    if (n == 0)
        return 0;
    A092674_ensure(n);
    if (A092674_max < 1) {
        A092674_cache[1] = 0;
        A092674_max = 1;
    }
    for (idx_t m = A092674_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t j = 1; j < m; ++j)
            sum += A092674_cache[j] * (seq_t)(m / j);
        A092674_cache[m] = A000217_a(m) - sum;
    }
    if (n > A092674_max)
        A092674_max = n;
    return A092674_cache[n];
}

seq_t a(idx_t n)
{
    return A000217_a(n) - A092674_a(n);
}
