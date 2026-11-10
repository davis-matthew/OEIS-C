/*
 * A064786  Inverse permutation to A054084.
 * https://oeis.org/A064786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = position of n in A054084, where A054084(2k-1)=p(k)+k,
 *            A054084(2k)=p(k), p(k)=least unused natural number (computed by
 *            simulation)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static size_t *A064786_pos = NULL;
static size_t A064786_cap = 0;
static size_t A064786_k = 0;
static size_t A064786_cand = 1;

static void A064786_ensure(size_t i)
{
    if (i < A064786_cap) return;
    size_t nc = A064786_cap ? A064786_cap : 64;
    while (nc <= i) nc <<= 1;
    size_t *t = (size_t *)realloc(A064786_pos, nc * sizeof(size_t));
    if (!t) abort();
    for (size_t j = A064786_cap; j < nc; ++j) t[j] = 0;
    A064786_pos = t;
    A064786_cap = nc;
}

seq_t a(idx_t n)
{
    size_t t = (size_t)n;
    A064786_ensure(t);
    while (A064786_pos[t] == 0) {
        A064786_k++;
        size_t k = A064786_k;
        A064786_ensure(A064786_cand);
        while (A064786_pos[A064786_cand] != 0) {
            A064786_cand++;
            A064786_ensure(A064786_cand);
        }
        size_t p = A064786_cand;
        size_t q = p + k;
        A064786_ensure(q);
        A064786_pos[q] = 2 * k - 1;
        A064786_pos[p] = 2 * k;
        A064786_cand++;
    }
    return (seq_t)A064786_pos[t];
}
