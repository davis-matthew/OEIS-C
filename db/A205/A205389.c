/*
 * A205389  s(A205387), where s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(A205387(n)), where s(j) = (1/2)*C(2j,j) and A205387(n)
 *            is the index j of the first repeated residue of s(k) mod n
 *            (computed by search)
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A205389_mul(uint32_t *d, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)d[i] * m + carry;
        d[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) d[(*len)++] = (uint32_t)carry;
}

static void A205389_div(uint32_t *d, size_t *len, uint32_t v)
{
    uint64_t rem = 0;
    for (size_t i = *len; i-- > 0; ) {
        uint64_t cur = (rem << 32) | d[i];
        d[i] = (uint32_t)(cur / v);
        rem = cur % v;
    }
    while (*len > 1 && d[*len - 1] == 0) --*len;
}

static uint32_t A205389_mod(const uint32_t *d, size_t len, uint32_t m)
{
    uint64_t rem = 0;
    for (size_t i = len; i-- > 0; )
        rem = ((rem << 32) | d[i]) % m;
    return (uint32_t)rem;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    size_t *first = (size_t *)calloc(N, sizeof(size_t));
    size_t cap = N / 16 + 8;
    uint32_t *d = (uint32_t *)calloc(cap, sizeof(uint32_t));
    if (!first || !d) { free(first); free(d); return (seq_t)0; }
    size_t len = 1;
    d[0] = 1;
    size_t j = 1;
    for (size_t k = 1; k <= N + 1; ++k) {
        if (k > 1) {
            A205389_mul(d, &len, (uint32_t)(4 * k - 2));
            A205389_div(d, &len, (uint32_t)k);
        }
        size_t r = (size_t)A205389_mod(d, len, (uint32_t)n);
        if (!first[r]) first[r] = k;
        else { j = first[r]; break; }
    }
    free(first);
    free(d);
    seq_t s = 1;
    for (size_t i = 2; i <= j; ++i) {
        s = s * (seq_t)(4 * i - 2);
        s = s / (seq_t)i;
    }
    return s;
}
