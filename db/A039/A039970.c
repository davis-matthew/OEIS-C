/*
 * A039970  An example of a d-perfect sequence: a(2*n) = 0, a(2*n+1) =
 *          Catalan(n) mod 3.
 * https://oeis.org/A039970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 0, a(2*n+1) = Catalan(n) mod 3
 * Range:     u8 n <= 7430, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7431 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)   /* even indices */
        return (seq_t)0;

    idx_t k = (n - 1) >> 1;    /* compute Catalan(k) mod 3 */

    /* static cache for Catalan numbers modulo 3 */
    static seq_t *cat = NULL;      /* cat[i] = Catalan(i) mod 3 */
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t sz  = 0;          /* number of computed entries (>=1) */

    /* initialize on first call */
    if (!cat) {
        cap = 8;
        cat = mem_new(seq_t, cap);
        cat[0] = (seq_t)1;        /* Catalan(0) = 1 */
        sz  = 1;
    }

    /* enlarge cache if necessary */
    if (k + 1 > cap) {
        while (cap < k + 1)
            cap <<= 1;
        cat = mem_resize(seq_t, cat, cap);
    }

    /* compute missing values up to k */
    for (idx_t m = sz; m <= k; ++m) {
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i)
            sum += cat[i] * cat[m - 1 - i];
        cat[m] = sum % (seq_t)3;   /* store modulo 3 */
    }
    sz = k + 1;                      /* update computed size */

    return cat[k];
}
