/*
 * A038313  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*11^j.
 * https://oeis.org/A038313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) * 11^j where (i,j) are the row
 *            and column indices of the n-th term in the triangle read
 *            row‑by‑row (i ≥ 0, 0 ≤ j ≤ i). For a given n, the row i is the
 *            unique integer satisfying i(i+1)/2 ≤ n < (i+1)(i+2)/2, and j = n
 *            - i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper routines */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}
#include <stdint.h>              /* uint64_t, etc. */

/* Integer square root via binary search (floor) */
static unsigned long long
isqrt_u64(unsigned long long v)
{
    unsigned long long lo = 0, hi = v, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq <= v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine row i and column j of the n-th triangle entry */
    unsigned long long k = 8ULL * (unsigned long long)n + 1ULL;   /* 8n+1 */
    unsigned long long m = isqrt_u64(k);                           /* floor(sqrt(8n+1)) */
    idx_t i = (m - 1) / 2;                                         /* row index */
    idx_t start = i * (i + 1) / 2;                                 /* first index of row i */
    idx_t j = n - start;                                           /* column index within that row */

    /* Compute value = binom(i,j) * 10^(i-j) * 11^j */
    seq_t val = binomial(i, j);
    val *= pow10(i - j);
    val *= ipow_idx(11, j);
    return val;
}
