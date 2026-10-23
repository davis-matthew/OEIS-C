/*
 * A038323  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*9^j.
 * https://oeis.org/A038323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*11^(i-j)*9^j where i = floor( (√(8n+1)-1)/2
 *            ), j = n - i*(i+1)/2 (enumerating the triangle row‑by‑row)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 38, u64 n <= 139, u128 n <= 504,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static inline idx_t floor_sqrt_u128(__int128 x) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 34;   /* > sqrt(8*UINT64_MAX+1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;   /* floor sqrt */
}

seq_t a(idx_t n) {
    /* Determine row i */
    __int128 val = (__int128)8 * n + 1;
    idx_t m = floor_sqrt_u128(val);
    idx_t i = (m - 1) / 2;

    /* Column j */
    __int128 baseIdx = (__int128)i * (i + 1) / 2;
    idx_t j = n - (idx_t)baseIdx;

    /* Compute term */
    seq_t bin = binomial(i, j);
    seq_t pow11 = big_pow((seq_t)11, (seq_t)(i - j));
    seq_t pow9  = big_pow((seq_t)9,  (seq_t)j);
    seq_t temp  = seq_mul(bin, pow11);
    seq_t result = seq_mul(temp, pow9);
    return result;
}
