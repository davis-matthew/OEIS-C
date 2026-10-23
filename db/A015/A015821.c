/*
 * A015821  Numbers k such that phi(k + 5) | sigma(k).
 * https://oeis.org/A015821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that φ(k + 5) divides σ(k)
 * Range:     u8 n <= 15, u16 n <= 386, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                    /* for NULL */
#include "../utils/append_candidate.h" /* append_candidate */
#include "../utils/sigma_of.h"         /* sigma_of */
#include "../utils/phi.h"              /* phi */

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static idx_t   max_checked = 0;   /* largest k tested so far        */
    static idx_t   found       = 0;   /* number of terms found           */
    static seq_t * values      = NULL;/* array of found k values         */
    static size_t values_len  = 0;    /* number of cached values         */
    static size_t values_cap  = 0;    /* capacity of cached array        */

    if (n == 0)
        return 0;                     /* undefined for n == 0            */

    /* Return cached value if already present */
    if (n <= values_len)
        return values[n - 1];

    /* Find terms until we have at least n of them */
    while (found < n) {
        ++max_checked;                     /* next k to test                 */

        seq_t sig = sigma_of(max_checked);         /* σ(k)          */
        seq_t ph  = phi(max_checked + (idx_t)5);    /* φ(k+5)       */

        if (ph != 0 && sig % ph == 0) {
            append_candidate(&values, &values_len, &values_cap,
                             (seq_t)max_checked);
            ++found;
        }
    }

    return values[n - 1];
}
