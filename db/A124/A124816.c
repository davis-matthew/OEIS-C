/*
 * A124816  Product of Riordan array (1,x(1-x^2))^(-1) and number triangle
 *          T(n,k)=C(floor(k/2),n-k).
 * https://oeis.org/A124816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row,col) entry of the product of the Riordan array
 *            (1 , x(1‑x²))^(-1) and the triangle T(n,k)=C(floor(k/2),n‑k).
 *            The sequence is read row‑major from that product triangle
 * Range:     u8 n <= 45, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"               /* provides seq_t binomial(idx_t a, idx_t b) */

/* Compute the nth term of A124816. */
seq_t a(idx_t n)
{
    /* Locate the row r and column c of the 2‑dimensional array,
       where row r contains r+1 elements and cells are numbered
       consecutively starting at a(0).  That is:
           n = r*(r+1)/2 + c   with 0 <= c <= r. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t c = n - r * (r + 1) / 2;

    /* Special case: first element of the triangle */
    if (r == 0) {
        return (seq_t)1;
    }

    /* ----------  Powers of the compositional inverse  ---------- */
    /* s(x) = f_inv(x) where f(x) = x(1‑x²).  Only odd coefficients
       occur:  s[2m+1] = C(3m,m) / (2m+1).  Build array of size r+1. */
    seq_t *s = (seq_t *)calloc(r + 1, sizeof(seq_t));
    for (idx_t m = 0; 2 * m + 1 <= r; ++m) {
        seq_t v = binomial(3 * m, m);
        v /= (2 * m + 1);           /* exact division */
        s[2 * m + 1] = v;
    }

    /* Lcoeff[j] = coefficient of x^r in s(x)^j for 0 <= j <= r. */
    seq_t *Lcoeff = (seq_t *)calloc(r + 1, sizeof(seq_t));
    Lcoeff[0] = (r == 0 ? 1 : 0);   /* s^0 = 1 */

    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    prev[0] = 1;                     /* s^0 */
    for (idx_t pow = 1; pow <= r; ++pow) {
        /* Convolve prev with s to obtain coeffs of s^pow. */
        seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
        for (idx_t i = 0; i <= r; ++i) {
            if (!prev[i]) continue;
            for (idx_t j = 0; i + j <= r; ++j) {
                if (!s[j]) continue;
                curr[i + j] += prev[i] * s[j];
            }
        }
        Lcoeff[pow] = curr[r];   /* coefficient of x^r in s^pow */
        free(prev);
        prev = curr;
    }
    free(prev);
    free(s);

    /* ----------  Triangle T(n,k) = C(floor(k/2), n‑k)  ---------- */
    /* Compute a(n) = sum_{j=c}^{r} Lcoeff[j] * C(floor(c/2), j‑c). */
    seq_t result = 0;
    idx_t half = c / 2;
    for (idx_t j = c; j <= r; ++j) {
        seq_t t = binomial(half, j - c);
        if (!t) continue;
        result += Lcoeff[j] * t;
    }

    free(Lcoeff);
    return result;
}
