/*
 * A170482  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 41 * 40^(n-1) for 1 ≤ n < 45 a(n) = 39*a(n-1) +
 *            39*a(n-2) + … + 39*a(n-44) – 780*a(n-45) for n ≥ 45 The
 *            coefficients come from the rational generating function that
 *            defines A170482
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cache of the first 45 terms: a[0] … a[44]   */
    static seq_t cache[45];
    static int cache_inited = 0;     /* 0 = not ready, 1 = ready   */
    static idx_t last = 44;          /* largest n for which cache[n] is valid */

    /* Initialise the cache once.  For n < 45 we know the closed form
       a(n) = 41 * 40^(n-1).  The value for n = 0 is 1.               */
    if (!cache_inited) {
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        if (45 > 1) cache[1] = (seq_t)41;            /* a(1) = 41 */
        for (idx_t i = 2; i < 45; ++i) {
            cache[i] = cache[i-1] * (seq_t)40;          /* geometric step */
        }
        cache_inited = 1;
        last = 44;                              /* last cached index */
    }

    /* Return cached value if already known. */
    if (n <= last) return cache[n];

    /* Compute successive terms up to n using the linear recurrence
       of order 45.  We maintain the last 45 values in a sliding window.
       After computing a(k) we shift the window left by one position. */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* 39 * a(k-1) + … + 39 * a(k-44) */
        for (idx_t j = 1; j <= 44; ++j) {
            sum += (seq_t)39 * cache[44 - (j-1)];  /* cache[44] = a(k-1), … */
        }
        /* – 780 * a(k-45) */
        sum -= (seq_t)780 * cache[0];

        /* Shift window left: cache[0] becomes a(k-44), … */
        for (idx_t i = 0; i < 44; ++i) {
            cache[i] = cache[i+1];
        }
        cache[44] = sum;      /* cache[44] now holds a(k)   */
        last = k;
    }

    return cache[n];
}
