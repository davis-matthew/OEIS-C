/*
 * A130123  Infinite lower triangular matrix with 2^k in the right diagonal
 *          and the rest zeros. Triangle, T(n,k), n zeros followed by the term
 *          2^k. Triangle by columns, (2^k, 0, 0, 0, ...).
 * https://oeis.org/A130123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^k if n = k(k+3)/2 (k ≥ 0); otherwise a(n) = 0
 * Range:     u8 n <= 6, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 5150, big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t x) */
#include "../utils/pow2.h"         /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /*  s = 9 + 8*n  (disciminant of 2n = k^2 + 3k )   */
    seq_t s = (seq_t)9 + (seq_t)8 * n;
    seq_t r = floor_sqrt(s);

    /* if s is not a perfect square → no integer k exists */
    if (r * r != s) {
        return 0;
    }

    /* compute k = (r - 3)/2  (r is guaranteed to be ≥ 3 for a valid k) */
    seq_t k_seq = (r - (seq_t)3) / (seq_t)2;
    idx_t k = (idx_t)k_seq;     /* k fits in idx_t for all n in the range */

    return pow2(k);             /* 2^k */
}
