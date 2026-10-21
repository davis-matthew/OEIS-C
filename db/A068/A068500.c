/*
 * A068500  Sequence of Fibonacci numbers whose sum of decimal digits sets a
 *          new record.
 * https://oeis.org/A068500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A068500: the nth Fibonacci number whose decimal digit sum
   exceeds all previous such sums.  The function lazily generates Fibonacci
   numbers and keeps only the ones that break the current maximum digit sum. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static state retained between calls */
    static seq_t *cache = NULL;      /* cached Fibonacci numbers with record digit sums */
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t f_prev = 0, f_cur = 0; /* current pair of Fibonacci numbers */
    static seq_t max_sum = 0;          /* maximum digit sum seen so far */
    static bool init = false;

    /* one‑time initialization */
    if (!init) {
        const size_t init_cap = 128;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure – return zero */

        cache_cap = init_cap;
        cache_len = 1;
        cache[0]  = (seq_t)1;   /* a(1) = 1 */
        f_prev    = (seq_t)1;   /* start with Fibonacci 1, 1 */
        f_cur     = (seq_t)1;
        max_sum   = digit_sum(cache[0]);

        init = true;
    }

    /* generate Fibonacci numbers until we have at least n recorded terms */
    while (cache_len < (size_t)n) {
        seq_t f_next = f_prev + f_cur;  /* next Fibonacci number */
        f_prev = f_cur;
        f_cur  = f_next;

        seq_t sum = digit_sum(f_next);
        if (sum > max_sum) {            /* new record */
            if (cache_len == cache_cap) /* grow cache if needed */
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) break; /* allocation failure – stop */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = f_next;
            max_sum = sum;
        }
    }

    /* return the requested term if it exists */
    return (cache_len >= (size_t)n) ? cache[n - 1] : (seq_t)0;
}
