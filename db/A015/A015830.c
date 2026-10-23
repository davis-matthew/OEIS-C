/*
 * A015830  Numbers k such that phi(k + 10) | sigma(k).
 * https://oeis.org/A015830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 462, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A015830: Numbers k such that φ(k + 10) divides σ(k) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Return the n-th term (1‑based).  The computed terms are cached
       between calls.  */
    if (n == 0)
        return 0; /* undefined, but harmless */

    static seq_t *cache = NULL;      /* store found k's */
    static idx_t  cap   = 0;         /* allocated capacity of cache   */
    static idx_t  len   = 0;         /* number of terms currently stored */
    static idx_t  last_k = 0;        /* last examined k (close to last cached k) */

    /* Ensure the cache is large enough for n terms.  */
    if (len < n)
    {
        /* Expand cache if necessary.  */
        if (cap < n)
        {
            idx_t new_cap = cap ? cap << 1 : 64;
            if (new_cap < n)
                new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)          /* allocation failed – abort */
                return 0;
            cache = tmp;
            cap   = new_cap;
        }

        /* Find terms until we have n.  */
        for (idx_t idx = len + 1; idx <= n; ++idx)
        {
            while (1)
            {
                ++last_k;
                seq_t sigma = sigma_of(last_k);
                /* phi accepts idx_t, which is uint64_t, so cast is safe. */
                seq_t phi_val = phi((idx_t)last_k + 10);
                /* phi_val cannot be zero for positive numbers. */
                if (phi_val != 0 && sigma % phi_val == 0)
                {
                    cache[idx - 1] = last_k; /* store k at 1‑based position */
                    break;
                }
            }
        }
        len = n;
    }

    return cache[n - 1];
}
