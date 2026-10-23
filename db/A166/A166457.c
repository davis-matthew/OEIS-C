/*
 * A166457  Numbers n such that n*100+1 is prime.
 * https://oeis.org/A166457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer m such that 100*m+1 is prime
 * Range:     u8 n <= 1, u16 n <= 366, u32 n <= 61, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 (1*100+1 = 101 is prime) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* static cache for memoised terms */
static seq_t *cache = NULL;    /* array of computed a(k) */
static size_t cache_cap = 0;   /* current capacity of cache */
static size_t cache_len = 0;   /* number of terms stored in cache */
static idx_t  last_checked = 0;/* last integer m that was tested for 100*m+1 */

/* return the n-th term of sequence A166457 (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1; safe fallback */
        return (seq_t)0;

    /* fast return if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        ++last_checked;                          /* next candidate m */
        seq_t candidate = (seq_t)last_checked * (seq_t)100 + (seq_t)1;
        if (is_prime_big(candidate)) {            /* 100*m+1 is prime? */
            if (cache_len == cache_cap)          /* grow cache if needed */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = last_checked;   /* store the m value */
        }
    }

    return cache[n - 1];
}
