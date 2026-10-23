/*
 * A018543  Divisors of 603.
 * https://oeis.org/A018543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018543: Divisors of 603 in ascending order
   a(n) = the n‑th divisor of 603 (n ≥ 1).  For n > 6, a(n) = 0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The divisor list is computed only once. */
    static seq_t divs[128];
    static size_t cnt = 0;

    if (n < 1)          /* below the offset (1) */
        return 0;

    if (cnt == 0) {
        const idx_t target = (idx_t)603;
        idx_t i;
        /* Gather all divisors by trial division up to sqrt(603). */
        for (i = 1; i * i <= target; ++i) {
            if ((target % i) == 0) {
                divs[cnt++] = (seq_t)i;
                idx_t d = target / i;
                if (d != i) {
                    divs[cnt++] = (seq_t)d;
                }
            }
        }
        /* Sort the divisors ascending (size is tiny, bubble sort is fine). */
        size_t k;
        for (k = 0; k + 1 < cnt; ++k) {
            size_t m;
            for (m = k + 1; m < cnt; ++m) {
                if (divs[k] > divs[m]) {
                    seq_t tmp = divs[k];
                    divs[k] = divs[m];
                    divs[m] = tmp;
                }
            }
        }
    }

    if ((size_t)(n - 1) < cnt)
        return divs[n - 1];
    return 0;   /* no divisor for n > 6 */
}
