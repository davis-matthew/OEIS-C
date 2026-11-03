/*
 * A126781  Number of functions f:{1,2,...,n}->{1,2,...,n} such that Im(f)
 *          contains 6 fixed elements.
 * https://oeis.org/A126781
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            n^n-6*(n-1)^n+15*(n-2)^n-20*(n-3)^n+15*(n-4)^n-6*(n-5)^n+(n-6)^n
 *            for n ≥ 6
 * Range:     u8 none, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 19 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    seq_t pos = 0;   /* sum of terms with positive coefficients */
    seq_t neg = 0;   /* sum of terms with negative coefficients */

    /* Normalized place values for k=0..6 */
    static const int coeff[7] = { 1, 6, 15, 20, 15, 6, 1 };
    static const int sign[7]  = { +1, -1, +1, -1, +1, -1, +1 };

    for (int k = 0; k <= 6; ++k) {
        seq_t base = (seq_t)(n - k);
        seq_t pw = ipow_idx(base, n);            /* base^n */
        seq_t term = (coeff[k] == 1) ? pw : mul_small(pw, coeff[k]);

        if (sign[k] > 0)
            pos += term;
        else
            neg += term;
    }

    /* pos ≥ neg for all n ≥ 6, so unsigned subtraction is safe */
    return pos - neg;
}
