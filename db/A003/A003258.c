/*
 * A003258  The number m such that c'(m) = A005206(A003231(n)), where c'(m) =
 *          A249115(m) is the m-th positive integer not in A003231.
 * https://oeis.org/A003258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m with A249115(m) = A005206(A003231(n)); computed as v =
 *            A005206(A003231(n)), a(n) = v - #{k >= 1 : A003231(k) <= v},
 *            where A003231(k) = floor(k*(5+sqrt(5))/2) and A005206(j) =
 *            floor((j+1)/phi)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

/* A003231(k) = floor((5k + sqrt(5k^2))/2) */
static uint64_t a3258_B(uint64_t k) {
    unsigned __int128 kk = k;
    unsigned __int128 s = isqrt_u128(5 * kk * kk);
    return (uint64_t)((5 * kk + s) / 2);
}

/* A005206(j) = floor((j+1)(sqrt(5)-1)/2) */
static uint64_t a3258_G(uint64_t j) {
    unsigned __int128 m = (unsigned __int128)j + 1;
    unsigned __int128 s = isqrt_u128(5 * m * m);
    return (uint64_t)((s - m) / 2);
}

seq_t a(idx_t n) {
    uint64_t v = a3258_G(a3258_B((uint64_t)n));
    /* count k >= 1 with A003231(k) <= v; since A003231(k) < 4k, at least v/4 */
    uint64_t k = v / 4;
    while (a3258_B(k + 1) <= v) k++;
    return (seq_t)(v - k);
}
