/*
 * A176419  Triangle read by rows: T(n,k) = A022168(n,k)*n!/binomial(n,k) + 1
 *          - n!.
 * https://oeis.org/A176419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2) c = n - r(r+1)/2
 *            T(r,c) = 1 - r! + c!(r-c)! * Prod_{i=r-m+1}^{r} (4^i - 1) /
 *            Prod_{i=1}^{m} (4^i - 1), with m = min(c, r-c)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 37, u64 n <= 59, u128 n <= 124,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

#ifndef MAX_ROW
#define MAX_ROW 2000            /* enough for n up to about 10^7 */
#endif

#ifndef MAX_FACTORS
#define MAX_FACTORS 400
#endif

static idx_t precomputed = 0;
static seq_t pow4[MAX_ROW + 1];
static seq_t fact[MAX_ROW + 1];

static void expand_up_to(idx_t n)
{
    if (precomputed == 0) {
        fact[0] = (seq_t)1;
        pow4[0] = (seq_t)1;
        precomputed = 0;
    }
    while (precomputed < n && precomputed + 1 <= MAX_ROW) {
        idx_t i = ++precomputed;
        fact[i] = fact[i - 1] * (seq_t)i;
        pow4[i] = pow4[i - 1] * ((seq_t)4);
    }
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n ) r++;
    idx_t k = n - r * (r + 1) / 2;     /* column in the triangle */

    if (r > MAX_ROW) {
        /* Extend arrays if necessary (unlikely for the given limits) */
        return (seq_t)0;
    }
    expand_up_to(r);

    idx_t m = (k <= (r - k)) ? k : (r - k);

    /* Build factor lists */
    static seq_t num_factors[MAX_FACTORS];
    static seq_t den_factors[MAX_FACTORS];
    size_t num_len = 0, den_len = 0;

    /* k! part */
    for (idx_t i = 1; i <= k; ++i) {
        num_factors[num_len++] = (seq_t)i;
    }
    /* (r-k)! part */
    for (idx_t i = 1; i <= r - k; ++i) {
        num_factors[num_len++] = (seq_t)i;
    }
    /* numerator of ratio: indices r-m+1 .. r */
    for (idx_t i = r - m + 1; i <= r; ++i) {
        num_factors[num_len++] = pow4[i] - (seq_t)1;
    }
    /* denominator of ratio: indices 1 .. m */
    for (idx_t i = 1; i <= m; ++i) {
        den_factors[den_len++] = pow4[i] - (seq_t)1;
    }

    /* Cancel common factors */
    for (size_t i = 0; i < num_len; ++i) {
        for (size_t j = 0; j < den_len; ++j) {
            seq_t g = gcd(num_factors[i], den_factors[j]);
            if (g != (seq_t)1) {
                num_factors[i] /= g;
                den_factors[j] /= g;
            }
        }
    }

    /* Compute the remaining product */
    seq_t term = (seq_t)1;
    for (size_t i = 0; i < num_len; ++i) {
        term *= num_factors[i];
    }

    /* a(n) = 1 - r! + term */
    seq_t result = (seq_t)1 - fact[r] + term;
    return result;
}
