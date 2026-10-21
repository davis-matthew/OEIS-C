/*
 * A211063  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant >=3n.
 * https://oeis.org/A211063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 307, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 2×2 matrices with entries in {1,…,n}
   whose determinant is at least 3n.  The algorithm counts all
   possible products a·d and b·c, stores their frequencies in an
   array, and then for each product p1 counts how many products
   p2 satisfy p1 – p2 ≥ 3n.  This reduces the N⁴ search to two
   N² loops plus an O(N²) linear scan. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    /* If n is too small there can be no matrix with determinant ≥ 3n. */
    if (n <= 3) return 0;

    /* Convert index to a native integer type for array indexing. */
    size_t N = (size_t)n;

    /* The largest possible product a·d or b·c is N². */
    size_t maxProd = N * N;

    /* Allocate an array that holds the number of (x,y) pairs
       with x·y equal to the index.  Count can be stored in
       16‑bit unsigned integer because it never exceeds a few hundred. */
    uint16_t *h = (uint16_t *)malloc((maxProd + 1) * sizeof(uint16_t));
    if (!h) return 0;          /* allocation failure – return 0 as a safe value */

    /* Initialise the array to zero. */
    memset(h, 0, (maxProd + 1) * sizeof(uint16_t));

    /* Count all products a·d for 1 ≤ a,d ≤ N. */
    for (size_t a = 1; a <= N; ++a) {
        for (size_t d = 1; d <= N; ++d) {
            h[a * d]++;
        }
    }

    seq_t total = 0;   /* the result */
    seq_t pref  = 0;   /* running sum of h[1] + … + h[p – 3N] */

    /* 3N is the required difference between the two products. */
    size_t diff = 3 * N;

    /* Scan all possible values of the larger product p1.  For each
       p1 we add the contribution of all p2 ≤ p1 – 3N. */
    for (size_t p1 = 1; p1 <= maxProd; ++p1) {
        if (p1 > diff) {
            pref += h[p1 - diff];
        }
        total += (seq_t)h[p1] * pref;
    }

    free(h);
    return total;
}
