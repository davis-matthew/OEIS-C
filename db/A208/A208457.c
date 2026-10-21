/*
 * A208457  Expansion of x * f(-x) * f(-x^12)^3 * psi(-x^3) / psi(x^2) in
 *          powers of x where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A208457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(n-1) in the infinite product * ∏_{m≥1}
 *            (1 - x^m)^{α_m} with * α_m = 1 + [m even] + [m divisible by 3] +
 *            4[m divisible by 12] * - 2[m divisible by 4] - [m divisible by
 *            6] * (α_m takes the values 1,2,2,0,1,2,1,0,2,2,1,4 repeating
 *            every 12.) * * This product equals the gener...
 * Range:     i8 n <= 200, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   alpha_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alpha_func.h"

static seq_t *g = NULL;          /* coefficient array */
static size_t g_size = 0;        /* current array size (number of coefficients) */

/* compute all coefficients up to target (i.e. up to degree target) */
static void recompute_coefficients(size_t target)
{
    size_t new_size = target + 1;
    seq_t *new_g = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_g) return; /* out of memory – caller will handle lack of result */

    new_g[0] = 1;
    for (size_t i = 1; i < new_size; ++i) new_g[i] = 0;

    for (size_t m = 1; m <= target; ++m) {
        int alpha = alpha_func(m);
        for (int rep = 0; rep < alpha; ++rep) {
            for (size_t i = target; i >= m; --i) {
                new_g[i] = new_g[i] - new_g[i - m];
            }
        }
    }

    if (g) free(g);
    g       = new_g;
    g_size  = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t target = (size_t)(n - 1);      /* need coefficient at degree n-1 */

    if (target >= g_size) {
        recompute_coefficients(target);
    }

    return g[target];
}
