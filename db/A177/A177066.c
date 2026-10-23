/*
 * A177066  Determinant of the symmetric n X n matrix M defined by M(i,j) =
 *          gcd(2i-1,2j-1) for 1 <= i,j <= n.
 * https://oeis.org/A177066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=1..n} phi(2*k-1) (phi is Euler's totient)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Static cache of partial products a(0..max_cached) */
    static seq_t *prod_cache = NULL;
    static idx_t max_cached = 0;

    /* Initialise cache if first call */
    if (prod_cache == NULL) {
        prod_cache = (seq_t *)malloc(sizeof(seq_t));
        if (prod_cache == NULL) return 0;          /* allocation failure – unlikely */
        prod_cache[0] = 1;                         /* a(0) */
    }

    /* Return cached value if already computed */
    if (n <= max_cached)
        return prod_cache[n];

    /* Resize cache to hold values up to n */
    size_t newsize = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(prod_cache, newsize * sizeof(seq_t));
    if (tmp == NULL) return 0;                     /* allocation failure – unlikely */
    prod_cache = tmp;

    /* Compute product incrementally */
    seq_t product = prod_cache[max_cached];
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        /* phi of odd 2*k-1 */
        seq_t phi_val = phi((idx_t)(2 * k - 1));
        product *= phi_val;                       /* seq_t multiplication is defined for all builds */
        prod_cache[k] = product;
    }
    max_cached = n;
    return prod_cache[n];
}
