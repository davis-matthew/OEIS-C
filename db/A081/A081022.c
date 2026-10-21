/*
 * A081022  Even order Taylor coefficients at x = 0 of exp(
 *          -(-2^(1/3)+(-3*x^2+2)^(1/3))/(-3*x^2+2)^(1/3) ), odd order
 *          coefficients being equal to zero.
 * https://oeis.org/A081022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*(n-2)*(2*n-1)*a(n-1) -
 *            2*(2*n-3)*(2*n-1)*(27*n^2-135*n+172)*a(n-2) +
 *            (2*n-5)*(2*n-3)*(2*n-1)*(108*n^3-972*n^2+2928*n-2951)*a(n-3) -
 *            9*(n-4)*(n-3)*(2*n-7)*(2*n-5)*(2*n-3)*(2*n-1)*(3*n-11)*(3*n-10)*a(n-4)
 *            with a(1)=1, a(2)=15, a(3)=615, a(4)=48825
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    /* index offset is 1 */
    static seq_t *cache = NULL;   /* cache[1] holds a(1) */
    static idx_t cache_len = 0;   /* highest n currently cached (inclusive) */

    if (n < 1) return 0;  /* behaviour for n < 1 is irrelevant */

    /* Grow cache if needed */
    if (cache_len < n) {
        size_t new_size = (size_t)n + 1; /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* memory allocation failure – abort computation */
            return 0;
        }
        cache = tmp;

        /* Compute missing terms */
        for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
            if (k == 1) {
                cache[1] = (seq_t)1;
            } else if (k == 2) {
                cache[2] = (seq_t)15;
            } else if (k == 3) {
                cache[3] = (seq_t)615;
            } else if (k == 4) {
                cache[4] = (seq_t)48825;
            } else {
                seq_t sn = (seq_t)k;

                /* coefficients as in the recurrence */
                seq_t coeff1 = (seq_t)12 * (sn - (seq_t)2) *
                               ((seq_t)2 * sn - (seq_t)1);

                seq_t coeff2 = (seq_t)2 * ((seq_t)2 * sn - (seq_t)3) *
                               ((seq_t)2 * sn - (seq_t)1) *
                               ((seq_t)27 * sn * sn - (seq_t)135 * sn + (seq_t)172);

                seq_t coeff3 = ((seq_t)2 * sn - (seq_t)5) *
                               ((seq_t)2 * sn - (seq_t)3) *
                               ((seq_t)2 * sn - (seq_t)1) *
                               ((seq_t)108 * sn * sn * sn -
                                (seq_t)972 * sn * sn +
                                (seq_t)2928 * sn -
                                (seq_t)2951);

                seq_t coeff4 = (seq_t)9 *
                               (sn - (seq_t)4) * (sn - (seq_t)3) *
                               ((seq_t)2 * sn - (seq_t)7) *
                               ((seq_t)2 * sn - (seq_t)5) *
                               ((seq_t)2 * sn - (seq_t)3) *
                               ((seq_t)2 * sn - (seq_t)1) *
                               ((seq_t)3 * sn - (seq_t)11) *
                               ((seq_t)3 * sn - (seq_t)10);

                /* recurrence */
                seq_t term1 = coeff1 * cache[k - 1];
                seq_t term2 = coeff2 * cache[k - 2];
                seq_t term3 = coeff3 * cache[k - 3];
                seq_t term4 = coeff4 * cache[k - 4];

                cache[k] = term1 - term2 + term3 - term4;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
