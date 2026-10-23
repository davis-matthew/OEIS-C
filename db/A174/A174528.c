/*
 * A174528  Triangle T(n,m) = 2*A022168(n,m) - binomial(n, m), 0 <= m <= n,
 *          read by rows.
 * https://oeis.org/A174528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = 2*A022168(n,m) - binomial(n,m) (Gaussian binomial with
 *            q=4)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) enumerates this triangle row‑by‑row, offset 0. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"       /* seq_t pow4(idx_t exp) */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

/* Gaussian binomial coefficient [n over k]_4, 0≤k≤n. */
static seq_t gauss_q4(idx_t n, idx_t k)
{
    seq_t res = (seq_t)1;
    if (k > n - k)                 /* use symmetry */
        k = n - k;
    for (idx_t i = 1; i <= k; ++i) {
        /* term = (4^(n-i+1)-1) / (4^i-1) */
        seq_t num = pow4(n - i + 1);
        num = num - (seq_t)1;
        seq_t den = pow4(i);
        den = den - (seq_t)1;
        res = (res * num) / den;   /* intermediate stays integral */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    seq_t gauss = gauss_q4(r, c);
    seq_t binom = binomial(r, c);
    seq_t result = (gauss << 1) - binom;   /* 2*gauss - binom */
    return result;
}
