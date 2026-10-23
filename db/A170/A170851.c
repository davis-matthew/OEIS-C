/*
 * A170851  G.f.: Product_{k>=1} (1 + 3x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥1}(1 + 3x^(2^k-1) + 3x^(2^k))
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming, keeping two
   coefficient arrays to avoid picking both exponents from the same
   factor. This runs in O(n log n) time and requires O(n) extra memory. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate coefficient array for the current (old) polynomial. */
    seq_t *old = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!old) return 0;          /* allocation failure – should not happen */
    old[0] = 1;                  /* base polynomial is 1 */

    for (uint64_t k = 1; k <= 63; ++k) {
        uint64_t exp1 = (1ULL << k) - 1;  /* exponent 2^k-1 */
        uint64_t exp2 = 1ULL << k;        /* exponent 2^k */

        if (exp1 > n && exp2 > n)
            break;                     /* beyond required index */

        /* Allocate temporary array for the new polynomial. */
        seq_t *tmp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (!tmp) { free(old); return 0; }

        /* Start with old coefficients (case of '1' in the factor). */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = old[i];

        /* Add contributions from exponent exp1 (weight 3). */
        if (exp1 <= n) {
            for (idx_t i = exp1; i <= n; ++i) {
                seq_t add = (seq_t)((unsigned __int128)old[i - exp1] * 3);
                tmp[i] += add;
            }
        }

        /* Add contributions from exponent exp2 (weight 3). */
        if (exp2 <= n) {
            for (idx_t i = exp2; i <= n; ++i) {
                seq_t add = (seq_t)((unsigned __int128)old[i - exp2] * 3);
                tmp[i] += add;
            }
        }

        free(old);   /* old array no longer needed */
        old = tmp;   /* use the new polynomial for next iteration */
    }

    seq_t result = old[n];
    free(old);
    return result;
}
