/*
 * A053258  Coefficients of the '5th-order' mock theta function phi_0(q).
 * https://oeis.org/A053258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 704, u32 n <= 2196, u64 n <= 7501,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A053258: a(n) = coefficient of q^n in
   Sum_{m>=0} q^{m^2} * Prod_{k=1}^m (1+q^{2k-1})  */

#include <stdlib.h>
#include <math.h>

/* The function uses a simple generating‑function recurrence.
   For each m the polynomial `P_m(q) = Prod_{k=1}^m (1+q^{2k-1})`
   is built incrementally.  Its coefficients are added to the final
   array with an index shift of m^2.  The calculation is performed
   only once up to the largest n requested. */

seq_t a(idx_t n)
{
    static idx_t cached_max = 0;          /* largest n for which cache is valid   */
    static seq_t *cache   = NULL;         /* array of a[0..cached_max]            */

    /* Allocate / extend the cache if necessary */
    if (n + 1 > cached_max) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_cache) { /* Allocation failed – abort silently */
            return 0;
        }
        /* initialise cache entries to 0 */
        for (idx_t i = 0; i < new_size; ++i) {
            new_cache[i] = 0;
        }

        /* coefficient array for the current product P_m */
        seq_t *coeff = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!coeff) {
            free(new_cache);
            return 0;
        }
        for (idx_t i = 0; i < new_size; ++i) {
            coeff[i] = 0;
        }
        coeff[0] = 1;                     /* P_0(q) = 1 */

        /* Contribution from m = 0 */
        new_cache[0] = new_cache[0] + coeff[0];

        /* Determine the largest integer m with m^2 <= n */
        idx_t max_term = (idx_t)sqrt((double)n);
        while ((max_term + 1) * (max_term + 1) <= n) ++max_term;
        while (max_term * max_term > n) --max_term;

        /* Build products for m = 1 .. max_term and add contributions */
        for (idx_t m = 1; m <= max_term; ++m) {
            idx_t odd = 2 * m - 1;            /* exponent appearing in (1+q^odd) */

            /* Update coefficients of P_m in descending order */
            for (idx_t i = new_size - 1; i >= odd; --i) {
                coeff[i] = coeff[i] + coeff[i - odd];
            }

            /* Shift index by m^2 and accumulate into cache */
            idx_t shift = m * m;
            idx_t limit = n - shift;
            for (idx_t i = 0; i <= limit; ++i) {
                new_cache[i + shift] = new_cache[i + shift] + coeff[i];
            }
        }

        /* Clean up previous cache and replace it */
        free(coeff);
        free(cache);
        cache = new_cache;
        cached_max = new_size;
    }

    return cache[n];
}
