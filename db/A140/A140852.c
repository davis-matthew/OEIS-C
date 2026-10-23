/*
 * A140852  Primes of the form 210k + 59.
 * https://oeis.org/A140852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 210k + 59 (k ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* cache of computed primes */
    enum { MAX_CACHE = 20000 };   /* sufficient for n ≤ 10 000 */
    static seq_t cache[MAX_CACHE];
    static idx_t cache_len = 0;
    static idx_t next_k   = 0;   /* current k value for 210k+59 */

    while (cache_len < n) {
        seq_t candidate = ((seq_t)210 * (seq_t)next_k) + 59;
        if (candidate >= 2 && is_prime_big(candidate)) {
            cache[cache_len++] = candidate;
        }
        ++next_k;   /* move to next k */
    }

    return cache[n - 1];
}
