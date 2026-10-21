/*
 * A062275  Array A(n, k) = n^k * k^n, n, k >= 0, read by antidiagonals.
 * https://oeis.org/A062275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 95, u64 n <= 217,
 *            u128 n <= 543, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/********************************************************************
 * OEIS A062275: Array A(n,k) = n^k * k^n, read by antidiagonals.
 * 
 * The sequence a(n) is obtained by enumerating the pairs (n,k)
 * in order of increasing n+k.  For a given index n, let s be the
 * unique non‑negative integer such that
 *
 *       (s+1)(s+2)/2 > n  and  (s)(s+1)/2 ≤ n
 *
 * Then n lies on diagonal s.  Within that diagonal the order is
 *
 *       (s,0), (s-1,1), (s-2,2), … , (0,s)
 *
 * Hence if kidx = n - s(s+1)/2, the corresponding pair is
 *
 *       n_val = s - kidx   ,   k_val = kidx
 *
 * The value is n_val^k_val * k_val^n_val, with 0^0 defined to be 1.
 ********************************************************************/

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n) {
    /* find diagonal s such that (s+1)(s+2)/2 > n */
    idx_t s = 0;
    while ( ( (s + 1) * (s + 2) ) / 2 <= n )
        ++s;

    /* number of elements before this diagonal */
    idx_t before = s * (s + 1) / 2;
    /* position inside the diagonal */
    idx_t kidx = n - before;
    /* coordinates on the array */
    idx_t n_val = s - kidx;
    idx_t k_val = kidx;

    /* compute n_val^k_val * k_val^n_val */
    seq_t pow1 = pow_uint((uint64_t)n_val, k_val);
    seq_t pow2 = pow_uint((uint64_t)k_val, n_val);
    return pow1 * pow2;
}
