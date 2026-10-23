/*
 * A037332  Numbers whose base-7 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑7 and base‑9 digit sums are equal.
   Computed by brute‑force search, caching results for arbitrary‑order calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *term_cache = NULL;   /* array of found terms             */
static size_t cache_cap   = 0;      /* current capacity of term_cache   */
static size_t cache_len   = 0;      /* number of terms stored so far    */
static seq_t last_value   = 0;      /* last integer examined, will be 0 */

/* Sum of digits of x in the specified base (base < 10). */
static seq_t digit_sum_base(seq_t x, int base)
{
    seq_t sum = 0;
    while (x) {
        sum += x % (seq_t)base;
        x   /= (seq_t)base;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* OEIS indices start at 1; 0 is unused */
        return 0;

    /* Lazy initialization of the cache */
    if (!term_cache) {
        cache_cap = 64;                        /* initial capacity     */
        term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!term_cache)                      /* allocation failure   */
            return 0;
    }

    /* Extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Examine the next integer candidate */
        seq_t candidate = last_value + 1;
        last_value = candidate;

        /* Check the property */
        if (digit_sum_base(candidate, 7) == digit_sum_base(candidate, 9)) {
            /* Grow cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!tmp)            /* allocation failure   */
                    return 0;
                term_cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the found term */
            term_cache[cache_len++] = candidate;
        }
    }

    return term_cache[n - 1];
}
