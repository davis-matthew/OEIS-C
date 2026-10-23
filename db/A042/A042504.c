/*
 * A042504  Numerators of continued fraction convergents to sqrt(780).
 * https://oeis.org/A042504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 782*a(n-4) - a(n-8) a(0)=27, a(1)=28, a(2)=363, a(3)=391,
 *            a(4)=21477, a(5)=21868, a(6)=283893, a(7)=305761
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values       */
static idx_t cache_len = 0;          /* allocated size of the array   */
static idx_t computed = 0;           /* next index that has not yet been computed */

/* Ensure that cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_len >= n + 1)
        return;                     /* already big enough */

    /* Grow the cache; start with a small size if empty, otherwise double */
    idx_t new_len = cache_len ? cache_len * 2 : 16;
    while (new_len < n + 1)
        new_len *= 2;

    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)new_len * sizeof(seq_t));
    if (!tmp && new_len > 0)         /* realloc failed */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_len = new_len;
}

/* Compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n < computed)          /* already computed */
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = computed; i <= n; ++i)
    {
        if (i < 8)             /* initial terms */
        {
            switch ((int)i)
            {
                case 0: cache[i] = (seq_t)27;   break;
                case 1: cache[i] = (seq_t)28;   break;
                case 2: cache[i] = (seq_t)363;  break;
                case 3: cache[i] = (seq_t)391;  break;
                case 4: cache[i] = (seq_t)21477;break;
                case 5: cache[i] = (seq_t)21868;break;
                case 6: cache[i] = (seq_t)283893;break;
                case 7: cache[i] = (seq_t)305761;break;
                default: break;
            }
        }
        else                    /* recurrence */
        {
            cache[i] = ((seq_t)782 * cache[i - 4]) - cache[i - 8];
        }
    }
    computed = n + 1;          /* all indices up to n are now computed */
    return cache[n];
}
