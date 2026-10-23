/*
 * A104001  Triangle T(n,k) read by rows: number of permutations in S_n
 *          avoiding all k-length patterns starting with fixed m, 2<k<=n,
 *          1<=m<=k.
 * https://oeis.org/A104001
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th entry of the triangle T(n,k) = (k-2)! *
 *            (k-1)^(n+1-k) read by rows, starting with n = 3
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 77, u64 n <= 206,
 *            u128 n <= 584, big any n
 * Helpers:   idx_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/idx_isqrt.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;                     /* offset is 3, behaviour below is irrelevant */

    idx_t t = n - 3;                         /* zero‑based index into the flattened triangle */

    /* Find the row r such that S(r) <= t < S(r)+(r-2)
       where S(r) = (r-3)(r-2)/2 is the number of entries before row r. */
    idx_t D = 1 + 8 * t;                     /* D = 1 + 8t, guaranteed to fit in idx_t for n up to ~10^9 */
    idx_t sqrtD = idx_isqrt(D);
    idx_t r = (5 + sqrtD) >> 1;              /* floor((5 + sqrt(D))/2) */
    if (r < 3) r = 3;                        /* safety for very small n */

    /* Compute the offset within row r */
    idx_t S_r = ((r - 3) * (r - 2)) / 2;      /* entries before row r */
    idx_t offset = t - S_r;                  /* 0‑based position in row r */
    idx_t k = 3 + offset;                    /* column index (k runs from 3 to r) */

    /* Compute factorial (k-2)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= k - 2; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute (k-1)^(r+1-k) using binary exponentiation */
    seq_t pow_val = 1;
    seq_t base = (seq_t)(k - 1);
    idx_t exp = r + 1 - k;                    /* exponent is non‑negative */
    while (exp > 0) {
        if (exp & 1) pow_val *= base;
        exp >>= 1;
        if (exp) base *= base;                /* avoid an extra multiply when exp==0 */
    }

    return fact * pow_val;
}
