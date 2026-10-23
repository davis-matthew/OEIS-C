/*
 * A121008  Numerators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(5*3^2) = 1/45.
 * https://oeis.org/A121008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121008
   a(n) = numerator of r(n)
   r(n) = Σ_{k=0}^{n} (-1)^k * C(k) / 45^k     // C(k) = Catalan number
   The fraction is reduced to lowest terms; a(n) is the reduced numerator.
*/

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Static arrays to cache intermediate values. */
    static seq_t *N = NULL;          /* N[k] = r(k) * 45^k */
    static idx_t Ncap = 0;           /* allocated capacity of N */
    static idx_t last = (idx_t)-1;   /* highest index computed so far */

    /* Grow the array if needed. */
    if (Ncap <= n) {
        idx_t newcap = Ncap ? Ncap << 1 : 1;
        while (newcap <= n) newcap <<= 1;
        N = (seq_t *)realloc(N, newcap * sizeof(seq_t));
        for (idx_t i = Ncap; i < newcap; ++i) N[i] = 0;
        Ncap = newcap;
    }

    /* Initialise if this is the first call. */
    if (last == (idx_t)-1) {
        N[0] = 1;                 /* r(0) = 1  →  N[0] = 1 */
        last = 0;
    }

    /* Compute N[k] for all new indices up to n. */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t cat = catalan(k);   /* Catalan number C(k) */
        if (k & 1) {
            /* odd k : sign = -1 */
            N[k] = 45 * N[k - 1] - cat;
        } else {
            /* even k : sign = +1 */
            N[k] = 45 * N[k - 1] + cat;
        }
    }
    last = n;

    /* Reduce N[n] by the gcd with 45^n = 5^n * 3^(2n). */
    seq_t tmp = N[n];
    idx_t exp5 = 0, exp3 = 0;

    /* Count factors of 5. */
    while (tmp % 5 == 0) {
        tmp /= 5;
        ++exp5;
    }
    /* Count factors of 3. */
    while (tmp % 3 == 0) {
        tmp /= 3;
        ++exp3;
    }

    /* How many of these factors are in the denominator 45^n? */
    idx_t take5 = (exp5 < n) ? exp5 : n;
    idx_t take3 = (exp3 < (2 * n)) ? exp3 : 2 * n;

    /* Apply the division to obtain the reduced numerator. */
    seq_t result = N[n];
    for (idx_t i = 0; i < take5; ++i) result /= 5;
    for (idx_t i = 0; i < take3; ++i) result /= 3;

    return result;
}
