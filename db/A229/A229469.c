/*
 * A229469  Numbers k such that T(k) + S(k) + 1 is prime, where T(k) and S(k)
 *          are the k-th triangular and square numbers.
 * https://oeis.org/A229469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that k*(k+1)/2 + k^2 + 1 is prime
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 4922, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_k   = 0;   /* previously checked k (0 means none) */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but be safe for n==0 */
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        ++next_k;
        seq_t k = (seq_t)next_k;

        /* expr = T(k) + S(k) + 1 = k*(k+1)/2 + k^2 + 1 */
        seq_t expr = (k * (k + 1)) / 2 + (k * k) + 1;

        if (isPrimeGeneric(expr))
            append_candidate(&cache, &cache_len, &cache_cap, k);
    }

    return cache[n - 1];
}
