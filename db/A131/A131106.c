/*
 * A131106  Rectangular array read by antidiagonals: k objects are each put
 *          into one of n boxes, independently with equal probability. a(n, k)
 *          is the expected number of boxes with exactly one object (n, k >=
 *          1). Sequence gives the numerators.
 * https://oeis.org/A131106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn, tabl
 * Formula:   a(n) = k * (1 - 1/n)^(k-1) read by anti‑diagonals
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Convert index to (n,k) pair read by anti‑diagonals. */
static void nk_from_index(idx_t idx, idx_t *n, idx_t *k)
{
    idx_t m = 0;
    idx_t sum = 0;
    while (sum < idx) {
        ++m;
        sum += m;
    }
    /* sum is triangular_m.  triangular_{m-1} = sum - m */
    idx_t prev_sum = sum - m;
    idx_t offset = idx - prev_sum;           /* 1‑based within diagonal */
    *n = m - offset + 1;
    *k = offset;
}

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t p;
    uint32_t exp;
} factor_t;

/* Factorize a small integer (x >= 2) into prime exponents. */
static void factor_small(uint64_t x, factor_t *arr, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if ((x % p) == 0) {
            uint32_t e = 0;
            while ((x % p) == 0) {
                x /= p;
                ++e;
            }
            arr[c].p = p;
            arr[c].exp = e;
            ++c;
        }
    }
    if (x > 1) {
        arr[c].p = x;
        arr[c].exp = 1;
        ++c;
    }
    *cnt = c;
}

/* Compute the numerator of a(n,k) = k * ((n-1)/n)^(k-1). */
seq_t a(idx_t pos)
{
    idx_t n, k;
    nk_from_index(pos, &n, &k);

    if (n == 1) {
        return (k == 1) ? seq_one() : (seq_t)0;
    }

    /* Factor n and k.  Both are <= ~200 for pos <= 10000. */
    factor_t fac_n[32];
    factor_t fac_k[32];
    size_t cnt_n, cnt_k;
    factor_small((uint64_t)n, fac_n, &cnt_n);
    factor_small((uint64_t)k, fac_k, &cnt_k);

    seq_t result = seq_one();

    /* Cancel common factors between k and n^(k-1). */
    for (size_t i = 0; i < cnt_k; ++i) {
        uint64_t p = fac_k[i].p;
        uint32_t e_k = fac_k[i].exp;
        uint32_t e_n = 0;

        /* find exponent of p in n */
        for (size_t j = 0; j < cnt_n; ++j) {
            if (fac_n[j].p == p) {
                e_n = fac_n[j].exp;
                break;
            }
        }

        /* exponent of p in denominator is (k-1)*e_n */
        uint64_t denom_exp = (uint64_t)(k - 1) * e_n;
        uint64_t cancel_exp = e_k;
        if (cancel_exp > denom_exp) cancel_exp = denom_exp;

        uint32_t keep_exp = e_k - (uint32_t)cancel_exp;
        if (keep_exp > 0) {
            result = seq_mul(result, ipow_idx((seq_t)p, (idx_t)keep_exp));
        }
    }

    /* Multiply by (n-1)^(k-1).  (n-1) and n are coprime. */
    result = seq_mul(result, ipow_idx((seq_t)(n - 1), (idx_t)(k - 1)));

    return result;
}
