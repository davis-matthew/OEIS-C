/*
 * A076795  Partial sums of (2n-1)!!.
 * https://oeis.org/A076795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(2*n-2) - a(n-2)*(2*n-3); a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_CAP 20001

seq_t a(idx_t n)
{
    /* Cache values up to CACHE_CAP for fast repeated access. */
    static seq_t cache[CACHE_CAP];
    static idx_t max_computed = 0;

    /* If requested index is beyond the cache capacity, compute on the fly. */
    if (n >= CACHE_CAP) {
        if (n == 0) return (seq_t)0;
        if (n == 1) return (seq_t)1;
        seq_t a0 = (seq_t)0, a1 = (seq_t)1, an;
        for (idx_t i = 2; i <= n; ++i) {
            an = a1 * ((seq_t)(2 * i - 2)) - a0 * ((seq_t)(2 * i - 3));
            a0 = a1;
            a1 = an;
        }
        return an;
    }

    /* Initialise base cases on first call. */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        if (n == 0) return cache[0];
        cache[1] = (seq_t)1;
        max_computed = 1;
        if (n == 1) return cache[1];
    }

    /* Return cached value if already computed. */
    if (n <= max_computed) return cache[n];

    /* Compute up to the requested index, storing each intermediate result. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * ((seq_t)(2 * i - 2)) -
                   cache[i - 2] * ((seq_t)(2 * i - 3));
    }
    max_computed = n;
    return cache[n];
}
