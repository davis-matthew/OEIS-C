/*
 * A015821  Numbers k such that phi(k + 5) | sigma(k).
 * https://oeis.org/A015821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that φ(k + 5) divides σ(k)
 * Range:     u8 n <= 15, u16 n <= 386, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 996 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                    /* for NULL */
#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
        seq_t ph  = totient(max_checked + (idx_t)5);    /* φ(k+5)       */

        if (ph != 0 && sig % ph == 0) {
            append_candidate(&values, &values_len, &values_cap,
                             (seq_t)max_checked);
            ++found;
        }
    }

    return values[n - 1];
}
