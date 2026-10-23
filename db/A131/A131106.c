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
 * Helpers:   nk_from_index, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

#include <stdint.h>
#include <stddef.h>
#include "../utils/nk_from_index.h"

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
            result = seq_mul(result, seq_pow((seq_t)p, (idx_t)keep_exp));
        }
    }

    /* Multiply by (n-1)^(k-1).  (n-1) and n are coprime. */
    result = seq_mul(result, seq_pow((seq_t)(n - 1), (idx_t)(k - 1)));

    return result;
}
