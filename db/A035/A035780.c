/*
 * A035780  Coordination sequence for C_43 lattice.
 * https://oeis.org/A035780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^43
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t binom_small[44];
static seq_t binom_long[20001];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* compute binomial coefficients for (1+x)^43 */
        binom_small[0] = 1;
        for (idx_t i = 1; i <= 43; ++i)
            binom_small[i] = binom_small[i-1] * (44 - i) / i;

        /* compute binomial coefficients for (1-x)^-43:
           f(m) = C(42+m, m)    for m = 0 .. 20000  */
        binom_long[0] = 1;
        for (idx_t m = 1; m <= 20000; ++m)
            binom_long[m] = binom_long[m-1] * ((seq_t)(42 + m)) / m;

        initialized = 1;
    }

    /* For n up to 10000 this is safe: need 2*n <= 20000 */
    idx_t twon = 2 * n;
    seq_t result = 0;

    idx_t max_i = twon < 43 ? twon : 43;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t term = binom_small[i] * binom_long[twon - i];
        result += term;
    }
    return result;
}
