/*
 * A141997  Primes congruent to 21 mod 29.
 * https://oeis.org/A141997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 21 (mod 29) (computed by
 *            incremental search)
 * Range:     u8 n <= 2, u16 n <= 217, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* curl the recursion serial */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t current_candidate = 79;  /* first prime ≡ 21 (mod 29) */

    if (n == 0) return 0;  /* safe guard for offsets <1 */

    /* extend cache until we have at least n entries */
    while (cache_len < n) {
        if (is_prime_big(current_candidate))
            append_candidate(&cache, &cache_len, &cache_cap, current_candidate);
        current_candidate += 29;   /* next number in the arithmetic progression */
    }
    return cache[n - 1];
}
