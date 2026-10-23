/*
 * A156936  G.f. of the z^3 coefficients of the FP2 in the fourth column of
 *          the A156925 matrix.
 * https://oeis.org/A156936
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign, uned
 * Formula:   * a(0) = 0 * a(1) = 0 * a(n) = 58*a(n-1) - 1571*a(n-2) +
 *            26428*a(n-3) - 309755*a(n-4) * + 2689810*a(n-5) -
 *            17964865*a(n-6) + 94564560*a(n-7) * - 398823930*a(n-8) +
 *            1362709780*a(n-9) - 3799420462*a(n-10) * + 8679603176*a(n-11) -
 *            16269149542*a(n-12) + 24993226196*a(n-13) * -
 *            31349144530*a(n-14) + 3...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices before the first term, return 0; behaviour below the offset
       does not matter for the OEIS definition. */
    if (n < 2)
        return (seq_t)0;

    /* Recurrence coefficients: d[0] = 1, d[i] = -c_i for 1 ≤ i ≤ 15. */
    static const seq_t d[16] = {
        (seq_t) 1,
        -(seq_t)58ULL,
        (seq_t)1571ULL,
        -(seq_t)26428ULL,
        (seq_t)309755ULL,
        -(seq_t)2689810ULL,
        (seq_t)17964865ULL,
        -(seq_t)94564560ULL,
        (seq_t)398823930ULL,
        -(seq_t)1362709780ULL,
        (seq_t)3799420462ULL,
        -(seq_t)8679603176ULL,
        (seq_t)16269149542ULL,
        -(seq_t)24993226196ULL,
        (seq_t)31349144530ULL,
        -(seq_t)31885547728ULL
    };

    /* Numerator coefficients of the generating function up to z^18. */
    static const seq_t Ncoeff[19] = {
        (seq_t)0,
        (seq_t)0,
        -(seq_t)6ULL,
        (seq_t)106ULL,
        -(seq_t)2772ULL,
        (seq_t)76070ULL,
        -(seq_t)1087552ULL,
        (seq_t)8632650ULL,
        -(seq_t)40358780ULL,
        (seq_t)106452214ULL,
        -(seq_t)99774996ULL,
        -(seq_t)284430514ULL,
        (seq_t)1125952500ULL,
        -(seq_t)1581820542ULL,
        (seq_t)737716032ULL,
        (seq_t)414532350ULL,
        -(seq_t)357790500ULL,
        -(seq_t)81870750ULL,
        -(seq_t)1275750ULL
    };

    /* Allocate space for intermediate sequence values. */
    size_t len = (size_t)n + 1;
    seq_t *a_vals = (seq_t *)malloc(len * sizeof(seq_t));
    if (!a_vals)
        return (seq_t)0; /* memory error, unlikely for typical inputs */

    a_vals[0] = (seq_t)0;
    a_vals[1] = (seq_t)0; /* a(1) is zero since the gf starts at z^2 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        int use = (i < 15) ? (int)i : 15;          /* only previous terms that exist */
        for (int k = 1; k <= use; ++k) {
            sum += d[k] * a_vals[i - k];
        }
        seq_t rhs = (i <= 18) ? Ncoeff[i] : (seq_t)0;
        a_vals[i] = rhs - sum; /* d[0] == 1, so no division needed */
    }

    seq_t result = a_vals[n];
    free(a_vals);
    return result;
}
