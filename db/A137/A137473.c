/*
 * A137473  Positive numbers k such that k^3 +- (k-1)^2 are primes.
 * https://oeis.org/A137473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that both k^3 + (k−1)^2
 *            and k^3 − (k−1)^2 are prime
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Persistent cache of terms */
    static seq_t *cache        = NULL;
    static size_t cache_len    = 0;
    static size_t cache_cap    = 0;
    static idx_t  last_checked = 0;

    /* Generate terms until we have at least n entries */
    while (cache_len < n) {
        last_checked++;                  /* next k to test   */
        seq_t k         = (seq_t)last_checked;
        seq_t k3        = k * k * k;               /* k^3            */
        seq_t kminus1   = (seq_t)(last_checked - 1);
        seq_t km1sq     = kminus1 * kminus1;       /* (k-1)^2        */
        seq_t plus      = k3 + km1sq;              /* k^3 + (k-1)^2  */
        seq_t minus     = k3 - km1sq;              /* k^3 - (k-1)^2  */

        if (is_prime_big(plus) && is_prime_big(minus))
            append_candidate(&cache, &cache_len, &cache_cap, k);
    }
    return cache[n - 1];
}
