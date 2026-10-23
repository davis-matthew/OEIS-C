/*
 * A162321  Number of reduced words of length n in the Weyl group D_15.
 * https://oeis.org/A162321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in the product * f(15) * Π_{i=1}^{14}
 *            f(2i), * where f(m) = (1 - x^m)/(1 - x) = 1 + x + … + x^{m-1}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* static storage for the coefficients of the degree‑210 polynomial */
static seq_t coeff[211];
static bool coeff_inited = false;

static void init_coeff(void)
{
    /* temporary array holding the current polynomial during multiplication */
    seq_t cur[211];
    for (int i = 0; i < 211; ++i) cur[i] = 0;

    /* f(15) -> coefficients 1 for degrees 0…14 */
    for (int i = 0; i <= 14; ++i) cur[i] = 1;
    int cur_deg = 14;

    for (int i = 1; i <= 14; ++i) {
        int m = 2 * i;                /* degree of f(m) is m-1 = 2i-1 */
        seq_t next[211];
        for (int j = 0; j < 211; ++j) next[j] = 0;

        for (int a = 0; a <= cur_deg; ++a) {
            seq_t val = cur[a];
            for (int b = 0; b <= m - 1; ++b) {
                int idx = a + b;
                next[idx] += val;
            }
        }
        cur_deg += m - 1;
        for (int j = 0; j <= cur_deg; ++j) cur[j] = next[j];
    }

    /* copy the finished polynomial to the static array */
    for (int i = 0; i < 211; ++i) coeff[i] = cur[i];
    coeff_inited = true;
}

seq_t a(idx_t n)
{
    if (!coeff_inited) init_coeff();
    if (n >= 211) return 0;
    return coeff[(size_t)n];
}
