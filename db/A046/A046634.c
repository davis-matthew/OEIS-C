/*
 * A046634  Number of cubic residues mod 6^n.
 * https://oeis.org/A046634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 36*a(n-3) - 216*a(n-4) - 251*a(n-6) +
 *            1506*a(n-7) + 216*a(n-9) - 1296*a(n-10); initial values: a(0)=1,
 *            a(1)=6, a(2)=9, a(3)=35, a(4)=210, a(5)=1083, a(6)=6253,
 *            a(7)=37518, a(8)=222705, a(9)=1331099, a(10)=7986594
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;  /* number of values cached (max index + 1) */

/* Ensure that `cache` contains values up to index `n`. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;  /* already computed */

    /* We need at least the first 11 terms for the recurrence. */
    idx_t new_len = n + 1;
    if (new_len < 11)
        new_len = 11;

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp)
        abort();  /* allocation failed */
    cache = tmp;

    /* Initialise the base values once. */
    if (cache_len == 0) {
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)6;
        cache[2]  = (seq_t)9;
        cache[3]  = (seq_t)35;
        cache[4]  = (seq_t)210;
        cache[5]  = (seq_t)1083;
        cache[6]  = (seq_t)6253;
        cache[7]  = (seq_t)37518;
        cache[8]  = (seq_t)222705;
        cache[9]  = (seq_t)1331099;
        cache[10] = (seq_t)7986594;
        cache_len = 11;
    }

    /* Compute further terms using the recurrence. */
    idx_t start = cache_len;
    if (start <= 10)    /* recurrence starts at n = 10 but we already have value at 10 */
        start = 10;

    for (idx_t i = start; i <= n; ++i) {
        cache[i] =  6 * cache[i - 1]
                  + 36 * cache[i - 3]
                  - 216 * cache[i - 4]
                  - 251 * cache[i - 6]
                  + 1506 * cache[i - 7]
                  + 216 * cache[i - 9]
                  - 1296 * cache[i - 10];
    }

    cache_len = new_len;
}

/* External function requested. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
