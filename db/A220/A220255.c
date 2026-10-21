/*
 * A220255  Majority value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal and vertical neighbors in a random 0..2 n X 2
 *          array.
 * https://oeis.org/A220255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Formula (empirical recurrence with initial values):
   a(1) = 2,  a(2) = 10,  a(3) = 35,  a(4) = 132,  a(5) = 497,
   a(6) = 1881, a(7) = 7102, a(8) = 26812, a(9) = 101196,
   a(10) = 381984, a(11) = 1441824, a(12) = 5442322.
   For n > 12:
      a(n) = 3*a(n‑1) + 4*a(n‑2) - 4*a(n‑3)
             - 3*a(n‑6) - 2*a(n‑7) + 2*a(n‑8) + 2*a(n‑9)
*/
static const seq_t init_vals[13] = {0,
    (seq_t)2, (seq_t)10, (seq_t)35, (seq_t)132, (seq_t)497,
    (seq_t)1881, (seq_t)7102, (seq_t)26812, (seq_t)101196,
    (seq_t)381984, (seq_t)1441824, (seq_t)5442322};

static seq_t *cache = NULL;       /* cached values 1… */
static size_t capacity = 0;       /* current allocated size (>= max_computed) */
static size_t max_computed = 0;   /* largest n already computed */

/* enlarge cache so that indices up to n are valid */
static void grow_cache(size_t n)
{
    if (n <= capacity) return;
    size_t new_cap = capacity ? capacity * 2 : n;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – leave old cache */
    cache = tmp;
    capacity = new_cap;
    /* No need to initialise new slots; they will be filled later. */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n < 1 */

    /* Ensure capacity for the requested index */
    grow_cache((size_t)n);

    /* Fill the initial 12 terms if the cache is empty */
    if (max_computed == 0) {
        size_t upto = capacity < 12 ? capacity : 12;
        for (size_t i = 1; i <= upto; ++i) cache[i] = init_vals[i];
        max_computed = upto;
    }

    /* Compute any missing terms up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 12) {
            cache[i] = init_vals[i];
        } else {
            seq_t pos = 3 * cache[i-1] + 4 * cache[i-2]
                      + 2 * cache[i-8] + 2 * cache[i-9];
            seq_t neg = 4 * cache[i-3] + 3 * cache[i-6]
                      + 2 * cache[i-7];
            cache[i] = pos - neg;
        }
    }
    max_computed = (size_t)n;
    return cache[n];
}
