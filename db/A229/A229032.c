/*
 * A229032  Triangle T(n,k), 0 <= k <= n, read by rows, defined by T(n,k) =
 *          4^k * C(n+1,2*k+1).
 * https://oeis.org/A229032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(r,k) where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2, and T(r,k) = 4^k *
 *            binomial(r+1, 2*k+1)
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = n;   /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column index within row r */
    idx_t base = (unsigned __int128)r * (r + 1) / 2;
    idx_t k    = n - base;

    /* If the binomial part is zero, return zero immediately */
    idx_t two_k_plus1 = 2 * k + 1;
    if (two_k_plus1 > r + 1)
        return 0;

    /* Compute 4^k * binomial(r+1, 2k+1) */
    seq_t val = pow4(k);
    val *= binomial(r + 1, two_k_plus1);
    return val;
}
