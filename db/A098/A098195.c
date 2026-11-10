/*
 * A098195  Starting values x such that the map x -> A098189(x) enters any
 *          cycle of length 29.
 * https://oeis.org/A098195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th x >= 1 with A098190(x) = 29, i.e. the orbit of x
 *            under x -> A098189(x) enters a cycle of length 29. Found by
 *            search: iterate each x past its transient, then measure the
 *            cycle length
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 986 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A098195_MEMO (1u << 20)

static uint32_t *A098195_memo = 0;

static uint64_t A098195_f(uint64_t m)
{
    uint64_t r, phi = 1, su = 1, p;
    if (m <= 1) return 1 - 1 + 0 * m; /* A098189(1) = 2 - 1 = 1 */
    r = m;
    for (p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            uint64_t pe = 1, ph = 1;
            while (r % p == 0) {
                r /= p;
                ph = (pe == 1) ? (p - 1) : ph * p;
                pe *= p;
            }
            su *= (1 + pe);
            phi *= ph;
        }
    }
    if (r > 1) {
        su *= (1 + r);
        phi *= (r - 1);
    }
    return su - phi;
}

static uint64_t A098195_step(uint64_t m)
{
    if (m < A098195_MEMO) {
        if (!A098195_memo)
            A098195_memo = (uint32_t *)calloc(A098195_MEMO, sizeof(uint32_t));
        if (A098195_memo) {
            if (A098195_memo[m] == 0)
                A098195_memo[m] = (uint32_t)A098195_f(m);
            return A098195_memo[m];
        }
    }
    return A098195_f(m);
}

static int A098195_good(uint64_t x)
{
    uint64_t y = x, z;
    int i, len;
    for (i = 0; i < 150; i++) y = A098195_step(y);
    z = A098195_step(y);
    len = 1;
    while (z != y && len < 2000) {
        z = A098195_step(z);
        len++;
    }
    return z == y && len == 29;
}

static uint64_t *A098195_terms = 0;
static size_t A098195_cnt = 0, A098195_cap = 0;
static uint64_t A098195_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A098195_cnt < (size_t)n) {
        if (A098195_good(A098195_next)) {
            if (A098195_cnt == A098195_cap) {
                A098195_cap = A098195_cap ? A098195_cap * 2 : 1024;
                A098195_terms = (uint64_t *)realloc(A098195_terms, A098195_cap * sizeof(uint64_t));
            }
            A098195_terms[A098195_cnt++] = A098195_next;
        }
        A098195_next++;
    }
    return (seq_t)A098195_terms[n - 1];
}
