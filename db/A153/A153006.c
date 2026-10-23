/*
 * A153006  Toothpick sequence starting at the outside corner of an infinite
 *          square from which protrudes a half toothpick.
 * https://oeis.org/A153006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in G(x) = x * (1 + x) / (1 - x) *
 *            ∏_{k≥1} (1 + x^(2^k-1) + 2 * x^(2^k))
 * Range:     u8 n <= 25, u16 n <= 431, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* We need the coefficient of x^(n‑1) in the part without the leading x. */
    idx_t degree = n - 1;
    size_t sz = (size_t)degree + 1;

    /* Allocate the current coefficient array, zero‑initialized. */
    seq_t *coeff = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!coeff) return 0;          /* allocation failure → zero */
    coeff[0] = 1;                  /* start with H(x) = 1 */

    /* Multiply by (1 + x) / (1 - x)  → 1 + 2x + 2x² + … */
    {
        seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!next) { free(coeff); return 0; }

        seq_t pref = 0;             /* sum coeff[0 … i‑1] */
        for (size_t i = 0; i < sz; ++i) {
            seq_t c = coeff[i];
            seq_t add = (i != 0) ? (seq_t)2 * pref : (seq_t)0;
            next[i] = c + add;
            pref += c;
        }
        free(coeff);
        coeff = next;
    }

    /* Multiply successively by the factors          *
     * (1 + x^(2^k-1) + 2 * x^(2^k)),  k = 1,2,…  */
    for (size_t k = 1; ; ++k) {
        size_t p = (size_t)1 << k;        /* p = 2^k */
        if (p - 1 > (size_t)degree) break;

        size_t s1 = p - 1;   /* x^(2^k-1) exponent   */
        size_t s2 = p;       /* x^(2^k)   exponent   */

        seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!next) { free(coeff); return 0; }

        for (size_t i = 0; i < sz; ++i) {
            seq_t val = coeff[i];
            if (!val) continue;

            /* multiply by 1 */
            next[i] += val;

            /* multiply by x^s1 (coefficient 1) */
            if (i + s1 < sz) next[i + s1] += val;

            /* multiply by 2*x^s2 (coefficient 2) */
            if (i + s2 < sz) next[i + s2] += val * (seq_t)2;
        }
        free(coeff);
        coeff = next;
    }

    seq_t result = coeff[degree];
    free(coeff);
    return result;
}
