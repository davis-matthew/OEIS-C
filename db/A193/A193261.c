/*
 * A193261  McKay-Thompson series of class 18D for the Monster group with a(0)
 *          = -2.
 * https://oeis.org/A193261
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = coefficient of q^n in * η(q)^2 η(q^6) η(q^9) / (η(q^2)
 *            η(q^3) η(q^18)^2) * (offset –1, so a(–1) = 1). * The function
 *            evaluates the corresponding product * h(q) = ∏_{k≥1} (1 –
 *            q^k)^{e(k)} * where * e(k) = 2 – [k≡0(2)] – [k≡0(3)] + [k≡0(6)]
 *            + [k≡0(9)] – 2·[k≡0(18)] . * Then a(n) = h_{n+1}...
 * Range:     i8 n <= 163, i16 n <= 469, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/exponent.h"

/* global cache */
static seq_t *coeff = NULL;      /* coeff[i] = coefficient of q^i in h(q) */
static idx_t  coeff_size = 0;    /* number of allocated elements   */

/* build the coefficient array up to index maxIdx (inclusive) */
static void compute_coeff(idx_t maxIdx)
{
    /* allocate new array */
    size_t sz = (size_t)(maxIdx + 1);
    seq_t *c = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!c) exit(EXIT_FAILURE);
    c[0] = (seq_t)1;   /* h_0 = 1 */

    /* multiply by each factor (1 – q^k)^e(k) */
    for (idx_t k = 1; k <= maxIdx; ++k) {
        signed char e = exponent(k);
        if (e == 0) continue;
        for (int t = 0; t < e; ++t) {
            for (idx_t i = maxIdx; i >= k; --i) {
                c[(size_t)i] -= c[(size_t)(i - k)];
            }
        }
    }

    /* replace old cache */
    if (coeff) free(coeff);
    coeff = c;
    coeff_size = maxIdx + 1;
}

/* public interface */
seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;               /* undefined for n < –1 */

    idx_t need = n + 1;                        /* coefficient index needed */

    if (!coeff || coeff_size <= need)          /* enlarge cache as required */
        compute_coeff(need);

    return coeff[(size_t)need];
}
