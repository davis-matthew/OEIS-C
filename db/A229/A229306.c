/*
 * A229306  Numbers n such that A031971(6*n) <> n (mod 6*n).
 * https://oeis.org/A229306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with (Sum_{j=1..6k} j^(6k)) mod 6k <> k;
 *            found by a search, skipping multiples of earlier terms
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A229306_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A229306_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A229306_mulmod(r, b, m);
        b = A229306_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static int A229306_member(uint64_t n)
{
    uint64_t m = 6 * n;
    uint64_t s = 0;
    for (uint64_t k = 1; k <= m; ++k) {
        s += A229306_powmod(k, m, m);
        if (s >= m) s -= m;
    }
    return s != n % m;
}

static uint64_t *A229306_terms = 0;
static size_t A229306_cnt = 0, A229306_cap = 0;
static uint64_t A229306_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A229306_cnt < (size_t)n) {
        uint64_t c = A229306_next++;
        int skip = 0;
        int mult = 0;
        for (size_t i = 0; i < A229306_cnt; ++i) {
            if (c % A229306_terms[i] == 0) { mult = 1; break; }
        }
        if (mult) skip = 0;
        if (mult || A229306_member(c)) {
            if (A229306_cnt == A229306_cap) {
                A229306_cap = A229306_cap ? A229306_cap * 2 : 256;
                A229306_terms = (uint64_t *)realloc(A229306_terms, A229306_cap * sizeof(uint64_t));
            }
            A229306_terms[A229306_cnt++] = c;
        }
        (void)skip;
    }
    return (seq_t)A229306_terms[n - 1];
}
