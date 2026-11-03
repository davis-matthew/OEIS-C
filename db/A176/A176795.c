/*
 * A176795  Triangle read by rows: T(n, k) = 2^n*(q^k - 1)*(q^(n - k) - 1) +
 *          1, where q = 4.
 * https://oeis.org/A176795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^r * (4^k - 1) * (4^(r-k) - 1) + 1 * where r satisfies
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 * and k = n - r*(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 66, u64 n <= 253, u128 n <= 946,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 1224 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t offset = 0;

    /* Find the row r such that offset(r) <= n < offset(r+1) */
    while ((offset + r + 1) <= n) {
        r++;
        offset += r;   /* offset now equals r*(r+1)/2 */
    }

    idx_t k = n - offset;          /* column index within the row */

    seq_t pow2r = pow2(r);          /* 2^r */
    seq_t pow4k = pow4(k);          /* 4^k */
    seq_t pow4rk = pow4(r - k);     /* 4^(r-k) */

    seq_t term = (pow4k - 1) * (pow4rk - 1);
    term = term * pow2r + 1;       /* 2^r * (… ) + 1 */

    return term;
}
