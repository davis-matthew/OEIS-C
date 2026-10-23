/*
 * A038337  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*11^j.
 * https://oeis.org/A038337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) enumerates the lower‑triangular array T(i,j) = binom(i,j) *
 *            12^(i-j) * 11^j row‑wise. That is, let i be the unique integer
 *            with i(i+1)/2 ≤ n < (i+1)(i+2)/2 and j = n − i(i+1)/2, then a(n)
 *            = T(i,j)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 37, u64 n <= 124, u128 n <= 446,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

static inline __int128 tri128(idx_t k) {
    return (__int128)k * (k + 1) / 2;
}

seq_t a(idx_t n) {
    /* Find row i such that tri(i) ≤ n < tri(i+1) */
    idx_t low  = 0;
    idx_t high = 1;

    while (tri128(high) <= n) {
        if (high > (UINT64_MAX >> 1)) { /* guard against infinite loop */
            break;
        }
        high <<= 1;
    }

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri128(mid) <= n)
            low = mid;
        else
            high = mid;
    }

    idx_t i = low;
    idx_t start_i = (idx_t)(tri128(i));
    idx_t j = n - start_i;           /* 0 ≤ j ≤ i */

    /* Compute binomial(i,j) * 12^(i-j) * 11^j */
    seq_t res        = binomial(i, j);
    seq_t pow12      = big_pow((seq_t)12, (seq_t)(i - j));
    seq_t pow11      = big_pow((seq_t)11, (seq_t)j);

    res = seq_mul(res, pow12);
    res = seq_mul(res, pow11);
    return res;
}
