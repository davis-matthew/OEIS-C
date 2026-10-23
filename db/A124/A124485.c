/*
 * A124485  Numbers k such that both 2*k-1 and 4*k-1 are primes.
 * https://oeis.org/A124485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 2k–1 and 4k–1 are primes
 * Range:     u8 n <= 11, u16 n <= 885, u32 n <= 56, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *k_cache = NULL;      /* cache of found terms */
static size_t k_len  = 0;          /* number of cached terms   */
static size_t k_cap  = 0;          /* current capacity         */
static idx_t  last_k_checked = 1;  /* last k processed         */

/* enlarge the cache when needed */
static void ensure_k_capacity(size_t needed)
{
    if (k_cap < needed) {
        size_t newcap = k_cap ? k_cap << 1 : 1;
        while (newcap < needed) newcap <<= 1;
        k_cache = seq_realloc(k_cache, k_cap, newcap);
        k_cap   = newcap;
    }
}

/* find the next k > last_k_checked that satisfies the condition */
static void find_next_k(void)
{
    for (idx_t k = last_k_checked + 1; ; ++k) {
        seq_t p1 = ((seq_t)k * ((seq_t)2)) - ((seq_t)1);
        seq_t p2 = ((seq_t)k * ((seq_t)4)) - ((seq_t)1);
        if (is_prime_big(p1) && is_prime_big(p2)) {
            seq_t kval = (seq_t)k;
            ensure_k_capacity(k_len + 1);
            k_cache[k_len++] = kval;
            last_k_checked   = k;
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    while (k_len < (size_t)n)                  /* extend cache if needed */
        find_next_k();
    return k_cache[(size_t)n - 1];              /* return the nth term */
}
