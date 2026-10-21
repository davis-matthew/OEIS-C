/*
 * A213927  T(n,k) = (z*(z-1)-(-1+(-1)^(z^2 mod 3))*n+(1+(-1)^(z^2 mod
 *          3))*k)/2, where z=n+k-1; n, k > 0, read by antidiagonals.
 * https://oeis.org/A213927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = (z*(z-1)+2*pos)/2 if z mod 3 != 0 a(n) = (z*(z-1)+2*k)/2
 *            if z mod 3 == 0 where z = n+k-1 is the antidiagonal index, pos
 *            is the position of n on the diagonal (ascending n), k = z - pos
 *            + 1. This computes the self‑inverse permutation A213927
 * Range:     u8 n <= 254, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   find_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/find_z.h"

seq_t a(idx_t n) {
    /* Handle the allowed minimum offset; behaviour for n==0 is undefined. */
    uint64_t z = find_z((uint64_t)n);
    __uint128_t prev_sum = (__uint128_t)(z - 1U) * z / 2U;
    uint64_t sum_prev = (uint64_t)prev_sum;
    uint64_t pos = (uint64_t)n - sum_prev;          /* 1‑based position on diagonal */
    uint64_t k   = z - pos + 1U;

    seq_t z_val = (seq_t)z;
    seq_t tmp   = z_val * (z_val - (seq_t)1U);       /* z*(z-1) */

    if (z % 3U == 0U) {
        /* Even b(z): use k term */
        tmp += (seq_t)2U * (seq_t)k;
    } else {
        /* Odd b(z): use pos (n) term */
        tmp += (seq_t)2U * (seq_t)pos;
    }
    tmp /= (seq_t)2U;
    return tmp;
}
