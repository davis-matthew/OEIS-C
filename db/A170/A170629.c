/*
 * A170629  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1 ≤ n < 48: a(n) = 2 + 42 * Σ_{k=1}^{47} a(n−k)
 *            (missing terms are 0); a(48) = 1 + 42 * Σ_{k=1}^{47} a(48−k) −
 *            903 * a(0); for n ≥ 49: a(n) = 42 * Σ_{k=1}^{47} a(n−k) − 903 *
 *            a(n−48);
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static cache up to 20000 terms – sufficient for the intended use. */
    static seq_t cache[20000];
    static idx_t cache_len = 0;   /* number of terms currently stored */

    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Initialise cache with the first term if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Expand cache lazily – assume n ≤ 19999.  No dynamic allocation needed. */
    if (n >= cache_len) {
        /* Sum of the last up to 47 terms */
        seq_t sum47 = cache[0];   /* includes a(0) for computing a(1) */

        for (idx_t i = 1; i <= n; ++i) {
            /* For n ≥ 48 the window must exclude a(0), a(1), … as they move out */
            if (i >= 48) {
                /* Remove the element that exits the 47‑term window */
                sum47 -= cache[i - 48];
            }

            seq_t val;
            if (i < 48) {
                /* a(i) = 2 + 42 * (sum of the available previous terms) */
                val = (seq_t)2 + (seq_t)42 * sum47;
            } else if (i == 48) {
                /* a(48) = 1 + 42 * Σ_{k=1}^{47} a(48−k) − 903 * a(0) */
                val = (seq_t)1 + (seq_t)42 * sum47 - (seq_t)903 * cache[0];
            } else {
                /* a(i) = 42 * Σ_{k=1}^{47} a(i−k) − 903 * a(i−48) */
                val = (seq_t)42 * sum47 - (seq_t)903 * cache[i - 48];
            }

            cache[i] = val;
            /* Update window for next iteration */
            sum47 += val;
        }

        cache_len = n + 1;
    }

    return cache[n];
}
