/*
 * A024737  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=3 and a(2)=a(3)=1.
 * https://oeis.org/A024737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A024737
 * a(1) = 3, a(2) = a(3) = 1
 * a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1)
 * (1‑based indices; a(0) is unused)
 */

#include <stdlib.h>          /* malloc / realloc / abort */

static seq_t *cache = NULL;          /* dynamic cache of computed terms   */
static size_t cache_len = 0;         /* allocated length of cache         */

/* Ensure cache can hold values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                       /* allocation failure */

    /* Initialise new slots to zero (uncomputed). */
    for (size_t i = cache_len; i < new_len; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_len = new_len;

    /* Initialise the three known base values if they are still zero. */
    if (cache_len > 1 && cache[1] == 0) cache[1] = (seq_t)3;
    if (cache_len > 2 && cache[2] == 0) cache[2] = (seq_t)1;
    if (cache_len > 3 && cache[3] == 0) cache[3] = (seq_t)1;
}

/* Return the nth term of A024737 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0 */

    ensure_cache(n);

    if (cache[n] != 0)
        return cache[n];               /* already known */

    /* For n > 3 the recurrence applies. */
    seq_t sum = 0;
    idx_t limit = (n + 1) / 4;          /* floor((n+1)/4) */

    for (idx_t i = 1; i <= limit; ++i) {
        idx_t idx1 = 2 * i - 1;
        idx_t idx2 = n - 2 * i + 1;
        sum += a(idx1) * a(idx2);
    }

    cache[n] = sum;
    return sum;
}
