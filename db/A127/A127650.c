/*
 * A127650  a(n) = n^n - (2n-1)!!.
 * https://oeis.org/A127650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - (2n-1)!!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_cache, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the OEIS offset). */
    if (n == 0)
        return 0;

    /* Static cache for double factorial (2k-1)!! */
    static seq_t *df_cache = NULL;
    static size_t df_cap   = 0;
    static idx_t  df_max   = 0;   /* highest n for which df_cache[n] is set */

    /* Ensure cache array has room for indices up to n. */
    ensure_cache(n, &df_cache, &df_cap);

    /* Compute double factorial up to n if needed. */
    if (n > df_max) {
        for (idx_t k = df_max + 1; k <= n; ++k) {
            if (k == 1) {
                df_cache[1] = 1;
            } else {
                df_cache[k] = df_cache[k-1] * ( (seq_t)(2*k - 1) );
            }
        }
        df_max = n;
    }

    /* Compute n^n using the special integer‑pow utility. */
    seq_t pow_val = seq_pow((seq_t)n, n);

    /* Subtract the double factorial. */
    return pow_val - df_cache[n];
}
