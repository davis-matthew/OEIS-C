/*
 * A095305  Numbers k such that A094020(k) < k.
 * https://oeis.org/A095305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A094020(k) < k, i.e. some j < k has
 *            k*bigomega(j) = j*bigomega(k); found by a search over k
 * Range:     u8 n <= 57, u16 n <= 9586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 7414 (2026-11-07)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

static idx_t *A095305_terms = NULL;
static size_t A095305_cnt = 0, A095305_cap = 0;
static idx_t A095305_next = 2;

static bool A095305_ok(idx_t k)
{
    idx_t om = (idx_t)bigomega(k);
    if (om == 0) return false;
    idx_t x = k, y = om;
    while (y) { idx_t t = x % y; x = y; y = t; }
    idx_t d = x;
    idx_t k0 = k / d, o0 = om / d;
    for (idx_t m = 1; m < d; ++m) {
        if ((idx_t)bigomega(m * k0) == m * o0) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A095305_cnt < (size_t)n) {
        idx_t k = A095305_next++;
        if (A095305_ok(k)) {
            if (A095305_cnt == A095305_cap) {
                A095305_cap = A095305_cap ? A095305_cap * 2 : 1024;
                A095305_terms = (idx_t *)realloc(A095305_terms, A095305_cap * sizeof(idx_t));
            }
            A095305_terms[A095305_cnt++] = k;
        }
    }
    return (seq_t)A095305_terms[n - 1];
}
