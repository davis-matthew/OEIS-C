/*
 * A173122  Irregular triangle T(n) = coefficients of Sum_{k=0..n} t(n,k,q)
 *          for powers of q, where t(n,k,q) = q*[n=2] + Sum_{j=0..5}
 *          q^j*binomial(n-2*j, k-j)*[n>2*j] with t(n,0,q) = t(n,n,q) = 1,
 *          read by rows.
 * https://oeis.org/A173122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   a(n) is the flat enumeration of coefficients of the irregular
 *            triangle A173122
 * Range:     u8 n <= 17, u16 n <= 61, u32 n <= 157, u64 n <= 349,
 *            u128 n <= 571, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given row r (0‑based) and position p (0‑based), a(n) = 2^r if p = 0, otherwise a(n) = 2^(r‑2p). */

#include <stdbool.h>
#include "../utils/seq_pow.h"

static inline seq_t pow2(idx_t e) {
    return seq_pow((seq_t)2, e);
}

seq_t a(idx_t n) {
    idx_t r, offset;

    if (n < 32) {   /* rows 0 … 10 are handled explicitly */
        idx_t acc = 0;
        for (r = 0;; ++r) {
            idx_t len;
            if (r <= 1)
                len = 1;
            else if (r == 2)
                len = 2;
            else   /* 3 ≤ r ≤ 10 */
                len = ((r - 1) / 2) + 1;
            if (acc + len > n)
                break;
            acc += len;
        }
        offset = acc;
    } else {        /* rows ≥ 11 – length is 6 each row */
        r = 11 + (n - 32) / 6;
        offset = 32 + 6 * (r - 11);
    }

    idx_t p = n - offset;   /* position within the row */

    if (p == 0)
        return pow2(r);
    else
        return pow2(r - 2 * p);
}
