/*
 * A127813  G.f.: (x^2+6*x^3+7*x^4+8*x^5+4*x^6-3*x^8-2*x^9-x^10) /
 *          ((1-x)^2*(1-x^2)^3*(1-x^3)^4*(1-x^4)).
 * https://oeis.org/A127813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 75, u64 n <= 975,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127813
   Generating function:
      (x^2+6*x^3+7*x^4+8*x^5+4*x^6-3*x^8-2*x^9-x^10) /
      ((1-x)^2*(1-x^2)^3*(1-x^3)^4*(1-x^4))
   Recurrence (for n ≥ 0):
      a(n) = N(n) - Σ_{i=1}^{24} D[i] * a(n-i)
   where N(n) is the coefficient of x^n in the numerator
   (0 for n>10) and D[i] are the coefficients of the denominator
   expansion.
   Initial values:
      a(0)=0, a(1)=0, a(2)=1, a(3)=8, a(4)=25, a(5)=72, a(6)=175,
      a(7)=384, a(8)=778, a(9)=1492, a(10)=2701, a(11)=4704... */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Denominator coefficients D[0..24] (D[0] = 1).  Only D[1]..D[24] are used in
   the recurrence. */
static const int d_coeff[25] = {
    1, -2, -2,  2,  7,  4, -14, -12,   3,  22,
   16, -14, -22, -14, 16,  22,   3, -12, -14,   4,
    7,   2, -2, -2,   1
};

/* Numerator coefficients N[0..10] (N[i] = 0 for i > 10). */
static const int n_coeff[11] = {
    0, 0, 1, 6, 7, 8, 4, 0, -3, -2, -1
};

typedef __int128 int128;   /* for intermediate signed arithmetic */

/* Return the nth term of A127813.  Results are cached for O(1) repeated
   lookup after initial computation. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Lazy allocation and initialization. */
    if (cap == 0) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;   /* a(0) = 0 */
        len    = 1;
    }

    /* Grow storage if needed. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute terms up to n if they are not yet cached. */
    if (len <= (size_t)n) {
        for (size_t k = len; k <= (size_t)n; ++k) {
            int nk = 0;
            if (k <= 10) nk = n_coeff[k];

            int128 sum = 0;
            for (int i = 1; i <= 24 && (size_t)i <= k; ++i) {
                seq_t prev = cache[k - i];
                int coeff  = d_coeff[i];
                int128 prod = (int128)coeff * (int128)prev;
                if (coeff > 0)          /* subtract positive coefficient */
                    sum -= prod;
                else                    /* add absolute value of negative coeff */
                    sum += -(int128)coeff * (int128)prev;
            }

            int128 val128 = (int128)nk + sum;   /* guaranteed non‑negative */
            cache[k] = (seq_t)val128;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
