/*
 * A093570  a(n) = Product_{k=1..n}(k + prime(k)).
 * https://oeis.org/A093570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n}(k + prime(k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The offset is 1, but we treat n==0 as an undefined request. */
    if (n == 0)
        return 0;  /* harmless return for out‑of‑bounds index */

    /* A static cache of the sequence up to the maximum index that has
       been computed.  The maximum n we expect to test is 10 000, so we
       pre‑allocate that many elements.  The array is statically
       zero‑initialized; for integer types the elements are zero,
       and for C++ big‑integer classes the default constructor is
       invoked automatically. */
    static seq_t cache[10001];
    static idx_t max_valid = 0;   /* number of terms that are cached */

    /* Return a previously cached value if available. */
    if (n <= max_valid)
        return cache[n - 1];

    /* Start multiplying from the last product we already have. */
    seq_t prod = (max_valid == 0) ? (seq_t)1 : cache[max_valid - 1];

    /* Compute the missing terms and store them in the cache. */
    for (idx_t k = max_valid + 1; k <= n; ++k) {
        seq_t term = (seq_t)k + (seq_t)get_prime(k);
        prod = prod * term;              /* use the type’s * operator */
        cache[k - 1] = prod;
    }

    /* Update the cache size and return the desired term. */
    max_valid = n;
    return cache[n - 1];
}
