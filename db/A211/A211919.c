/*
 * A211919  Number of (w,x,y,z) with all terms in {1,...,n} and w*x>=3*y*z.
 * https://oeis.org/A211919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (w,x,y,z) with 1≤w,x,y,z≤n and w*x ≥ 3*y*z
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each product p = w*x compute number of pairs (y,z)
   with product ≤ floor(p/3). This requires a prefix sum of the
   frequency of products y*z.  Complexity O(n²) time and
   O(n²/3) memory. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* maximum product of y*z we need to consider */
    idx_t Nmax = (n * n) / 3;   /* floor division is safe */
    size_t size = (size_t)Nmax + 1UL;   /* array length */

    /* count frequencies of products y*z up to Nmax */
    uint16_t *freq = (uint16_t *)malloc(size * sizeof(uint16_t));
    if (!freq) return (seq_t)0; /* allocation failed, return 0 */
    /* initialise to zero */
    for (size_t i = 0; i < size; ++i) freq[i] = 0U;

    for (idx_t y = 1; y <= n; ++y) {
        for (idx_t z = 1; z <= n; ++z) {
            idx_t prod = y * z;
            if (prod <= Nmax) {
                /* freq[prod] will never overflow 16 bits */
                ++freq[prod];
            }
        }
    }

    /* build prefix sums of frequencies (prefix[i] = #pairs with product ≤ i) */
    uint32_t *pref = (uint32_t *)malloc(size * sizeof(uint32_t));
    if (!pref) {
        free(freq);
        return (seq_t)0;
    }

    uint32_t acc = 0U;
    pref[0] = 0U;
    for (idx_t i = 1; i <= Nmax; ++i) {
        acc += freq[i];
        pref[i] = acc;
    }

    free(freq);   /* frequency table no longer needed */

    /* accumulate answer */
    seq_t ans = (seq_t)0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t prod = w * x;          /* w*x fits in idx_t for n≤10000 */
            idx_t limit = prod / 3;
            if (limit > Nmax) limit = Nmax;  /* safety for small n */
            ans += (seq_t)pref[limit];
        }
    }

    free(pref);
    return ans;
}
