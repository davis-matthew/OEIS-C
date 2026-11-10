/*
 * A073831  Maximum of A073830(k) for k between A001359(n) and A001359(n+1).
 * https://oeis.org/A073831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max { A073830(k) : A001359(n) < k < A001359(n+1) }, where
 *            A073830(k) = (4*((k-1)! + 1) + k) mod (k*(k+2)); twin primes are
 *            found by search, and each A073830(k) is computed directly
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-07)
 * Helpers:   is_prime, mulmod_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod_idx_to_seq.h"
#include "../utils/is_prime.h"

static uint64_t *A073831_tw = NULL;
static size_t A073831_cnt = 0, A073831_cap = 0;
static uint64_t A073831_cand = 3;

static uint64_t A073831_twin(uint64_t n)
{
    while (A073831_cnt < (size_t)n) {
        uint64_t c = A073831_cand;
        if (is_prime((idx_t)c) && is_prime((idx_t)(c + 2))) {
            if (A073831_cnt == A073831_cap) {
                size_t nc = A073831_cap ? A073831_cap * 2 : 256;
                uint64_t *t = (uint64_t *)realloc(A073831_tw, nc * sizeof(uint64_t));
                if (!t) return 0;
                A073831_tw = t;
                A073831_cap = nc;
            }
            A073831_tw[A073831_cnt++] = c;
        }
        A073831_cand += 2;
    }
    return A073831_tw[n - 1];
}

static seq_t A073831_f(uint64_t k)
{
    seq_t mod = (seq_t)k * ((seq_t)k + 2);
    if (mod == 0) return 0;   /* only possible on overflow in narrow builds */
    seq_t fact = 1;
    for (uint64_t i = 2; i < k; ++i)
        fact = mulmod_idx_to_seq(fact, (idx_t)i, mod);
    seq_t value = 4 * (fact + 1) + (seq_t)k;
    return value % mod;
}

seq_t a(idx_t n)
{
    uint64_t p = A073831_twin((uint64_t)n);
    uint64_t q = A073831_twin((uint64_t)n + 1);
    seq_t best = 0;
    for (uint64_t k = p + 1; k < q; ++k) {
        seq_t v = A073831_f(k);
        if (v > best) best = v;
    }
    return best;
}
