/*
 * A212051  Number of (w,x,y,z) with all terms in {1,...,n} and 3w*x=5*y*z.
 * https://oeis.org/A212051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count quadruples (w,x,y,z), 1 ≤ w,x,y,z ≤ n, satisfying 3*w*x = 5*y*z.
   The algorithm builds the distribution of products w*x for all w,x ≤ n.
   For each integer t with 5*t ≤ n², it adds the product of the counts of
   w*x = 5*t and y*z = 3*t.  Complexity: O(n²) time, O(n²) space. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases quickly. */
    if (n < 1) return 0;

    /* Max product achievable with numbers ≤ n.  Use size_t to avoid overflow. */
    const size_t maxProd = (size_t)n * (size_t)n;

    /* Allocate count array for products w*x.  Use uint16_t; counts stay < 65535
       for n up to about 10000 (the intended limit). */
    uint16_t *cnt = (uint16_t *)calloc(maxProd + 1, sizeof(uint16_t));
    if (!cnt) return 0;  /* allocation failed – return 0 as a safe fallback */

    /* Build the distribution of products w*x. */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            size_t prod = (size_t)w * (size_t)x;
            cnt[prod]++;
        }
    }

    /* Sum over all t such that 5*t and 3*t are achievable products. */
    seq_t total = 0;
    const size_t tmax = maxProd / 5;  /* 5*t ≤ n²  ⇒  t ≤ n²/5 */
    for (size_t t = 1; t <= tmax; ++t) {
        size_t m1 = 5 * t;  /* target for w*x */
        size_t m2 = 3 * t;  /* target for y*z */
        if (m1 > maxProd || m2 > maxProd) continue;  /* safety, though unlikely */
        seq_t c1 = cnt[m1];
        seq_t c2 = cnt[m2];
        total += c1 * c2;
    }

    free(cnt);
    return total;
}
