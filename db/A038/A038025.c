/*
 * A038025  Winner of n-th Littlewood Frog Race.
 * https://oeis.org/A038025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A038025: Winner of the n‑th Littlewood Frog Race.
   For n ≥ 1 we search all k with 1 ≤ k ≤ n and gcd(n,k)=1.
   For each k we find the smallest prime of the form a·n + k (a ≥ 0)
   – i.e. the smallest prime congruent to k modulo n.
   The winner is the k that yields the largest such prime; if several k give
   the same prime we take the greatest k. */

/* The function below memoises results so that calls in any order are
   efficient, and it uses only the supplied utilities. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for valid calls; we do not care about n == 0. */
    if (n == 0)
        return 0;

    /* Memoisation cache: static dynamic array of seq_t. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* Cast the result of realloc so the code compiles under C++ as well. */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* Zero the newly added elements (cache[0] is unused). */
        if (new_size > cache_size) {
            size_t to_zero = (size_t)(new_size - cache_size) * sizeof(seq_t);
            memset((void *)((char *)tmp + cache_size * sizeof(seq_t)), 0, to_zero);
        }
        cache     = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed. */
    if (cache[n] != 0)
        return cache[n];

    seq_t n_seq  = (seq_t)n;
    seq_t bestP  = 0;
    idx_t bestK  = 0;

    /* Iterate over all coprime k. */
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd(n_seq, (seq_t)k) != 1)
            continue;

        /* Find the smallest prime ≡ k (mod n). */
        seq_t prime_candidate = (seq_t)k;
        while (!is_prime_big(prime_candidate))
            prime_candidate += n_seq;

        if (prime_candidate > bestP || (prime_candidate == bestP && k > bestK)) {
            bestP = prime_candidate;
            bestK = k;
        }
    }

    cache[n] = (seq_t)bestK;
    return (seq_t)bestK;
}
