/*
 * A081759  Numbers k such that 5*k+6 is prime.
 * https://oeis.org/A081759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th positive integer k such that 5*k+6 is prime. The
 *            function generates terms on demand by testing successive
 *            integers k and storing the found values. It uses the provided
 *            is_prime routine
 * Range:     u8 n <= 50, u16 n <= 7035, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t) */

/* Cache of found terms.  All indices and counts use idx_t, values are seq_t. */
static seq_t *cache = NULL;      /* array of k values  */
static idx_t cache_len = 0;      /* number of terms stored   */
static idx_t cache_cap = 0;      /* allocated capacity      */
static idx_t last_k = 0;         /* last k that was examined */

/* Resize the cache to hold at least one more element. */
static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term (1‑based).  If n==0 the value is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined offset */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_k;                           /* next candidate k      */
        idx_t p = 5 * last_k + 6;           /* test value 5*k+6      */

        if (is_prime(p)) {                  /* primality test       */
            grow_cache();
            cache[cache_len++] = (seq_t)last_k;  /* store k             */
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
