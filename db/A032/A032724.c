/*
 * A032724  Numbers k such that k prefixed by '8' and followed by '7' is
 *          prime.
 * https://oeis.org/A032724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 28, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10, seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032724: numbers k such that 8·10^{len(k)+1}+k·10+7 is prime */

/* The sequence is produced by scanning successive k starting from 1,
   testing primality of the 8*10^{len(k)+1}+k*10+7 expression, and
   memoising the qualifying k. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;    /* allocated size of cache */
static idx_t cache_count = 0;  /* number of stored values */
static idx_t next_k = 1;       /* next k to test */

/* Helper: grow the cache to at least new_cap entries */
static void grow_cache(idx_t new_cap)
{
    seq_t *newarr = seq_alloc(new_cap);
    if (!newarr) {
        /* Allocation failed – abort as the sequence cannot be continued */
        abort();
    }
    for (idx_t i = 0; i < cache_count; ++i)
        newarr[i] = cache[i];
    seq_free(cache);
    cache = newarr;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; value for n==0 is irrelevant */

    /* Return a stored value if already known */
    if (n <= cache_count)
        return cache[n - 1];

    /* Ensure the cache can grow */
    if (cache_count >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        grow_cache(new_cap);
    }

    /* Generate new terms until we reach the n‑th one */
    while (cache_count < n) {
        /* Construct the candidate number N = 8·10^{len(k)+1} + k·10 + 7 */
        int len_k = digits(next_k);
        seq_t pow10_val = pow10((idx_t)(len_k + 1));
        seq_t N = ((seq_t)8 * pow10_val) + (seq_t)next_k * 10 + 7;

        if (is_prime_big(N)) {
            /* Store the qualifying k */
            cache[cache_count++] = (seq_t)next_k;
            if (cache_count >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
                grow_cache(new_cap);
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}
