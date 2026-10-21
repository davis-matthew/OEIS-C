/*
 * A212111  Number of (w,x,y,z) with all terms in {1,...,n} and w*x+y*z<n^2.
 * https://oeis.org/A212111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with 1≤w,x,y,z≤n and w*x + y*z < n^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* N = n^2 - 1, the maximum allowed sum of products minus one */
    seq_t Nseq = (seq_t)n * (seq_t)n - (seq_t)1;
    size_t N = (size_t)Nseq;              /* fits into size_t for n ≤ 10000 */
    size_t size = N + 1;                  /* array indices 0..N */

    /* cnt[p] = number of ordered pairs (w,x) with product p (0 ≤ p ≤ N) */
    uint16_t *cnt = (uint16_t *)calloc(size, sizeof(uint16_t));
    if (!cnt)
        return 0;                         /* allocation failed */

    /* Build cnt by enumerating all product pairs */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            seq_t prod = (seq_t)w * (seq_t)x;
            if (prod <= Nseq)
                ++cnt[(size_t)prod];
        }
    }

    /* pref[i] = number of pairs (w,x) with product ≤ i (0 ≤ i ≤ N) */
    uint32_t *pref = (uint32_t *)calloc(size, sizeof(uint32_t));
    if (!pref) {
        free(cnt);
        return 0;
    }
    uint32_t running = 0;
    for (size_t i = 0; i < size; ++i) {
        running += cnt[i];
        pref[i] = running;
    }

    /* Compute the answer using the prefix sums */
    seq_t ans = 0;
    for (size_t p = 1; p < size; ++p) {   /* product 0 never occurs */
        uint16_t cp = cnt[p];
        if (cp == 0)
            continue;
        size_t limit = N - p;              /* largest allowed product for (y,z) */
        seq_t g = pref[limit];
        ans += (seq_t)cp * g;
    }

    free(cnt);
    free(pref);
    return ans;
}
