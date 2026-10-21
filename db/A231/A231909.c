/*
 * A231909  Number of 2 X n 0..2 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to itself plus one mod 3, with upper left element zero (rock
 *          paper and scissors drawn positions).
 * https://oeis.org/A231909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Linear recurrence a(1) = 3, a(2) = 17, a(3) = 137, a(4) = 948,
 *            a(5) = 6975, a(6) = 50323 a(n) = 9*a(n-1) - 5*a(n-2) - 66*a(n-3)
 *            + 78*a(n-4) + 4*a(n-5) - 16*a(n-6) for n > 6 The recurrence is
 *            evaluated using additions and a final subtraction to avoid
 *            negative intermediate values
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed values, index starts at 1 */
static idx_t  cache_limit = 0;    /* allocated size of cache          */
static idx_t  cache_max    = 0;    /* maximum n for which cache has a value */

/* Compute the nth term of A231909 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Grow cache if necessary */
    if (cache_limit < n + 1) {
        idx_t new_limit = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_limit * sizeof(seq_t));
        if (!tmp)
            return 0;              /* allocation failed */
        seq_cache = tmp;
        cache_limit = new_limit;
    }

    /* If we already know this term, return it immediately */
    if (n <= cache_max)
        return seq_cache[n];

    /* Compute values from the last one we have up to the requested n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        switch (i) {
            case 1: seq_cache[1] = 3;          break;
            case 2: seq_cache[2] = 17;         break;
            case 3: seq_cache[3] = 137;        break;
            case 4: seq_cache[4] = 948;        break;
            case 5: seq_cache[5] = 6975;       break;
            case 6: seq_cache[6] = 50323;      break;
            default:
            {
                /* Positives and negatives separated to keep intermediate
                   results non‑negative for unsigned seq_t types. */
                seq_t pos = 9 * seq_cache[i - 1]
                          + 78 * seq_cache[i - 4]
                          + 4 * seq_cache[i - 5];
                seq_t neg = 5 * seq_cache[i - 2]
                          + 66 * seq_cache[i - 3]
                          + 16 * seq_cache[i - 6];
                seq_cache[i] = pos - neg;
            }
        }
    }

    cache_max = n;
    return seq_cache[n];
}
