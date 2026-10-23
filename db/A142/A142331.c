/*
 * A142331  Primes congruent to 37 mod 45.
 * https://oeis.org/A142331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 37 (mod 45), offset 1
 * Range:     u8 n <= 2, u16 n <= 278, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple deterministic primality test for idx_t values */
static inline bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // behaviour for indices below the offset

    /* Cache of primes congruent to 37 mod 45 */
    static seq_t cache[20000];
    static idx_t cached_len = 0;
    static idx_t next_k = 0;       /* k such that candidate = 45*k + 37 */

    /* Generate more terms until we have at least n cached */
    while (cached_len < n)
    {
        idx_t candidate = 45 * next_k + 37;
        if (is_prime_idx(candidate))
            cache[cached_len++] = (seq_t)candidate;
        ++next_k;
    }

    return cache[n - 1];
}
