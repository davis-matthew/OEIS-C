/*
 * A085358  Runs of zeros in binomial(3k,k)/(2k+1) (mod 2): relates ternary
 *          trees (A001764) to the infinite Fibonacci word (A003849).
 * https://oeis.org/A085358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of consecutive zeros in the binary
 *            * sequence b(k) = binomial(3k, k)/(2k+1) (mod 2). a(0) is the
 *            length of the * first run of zeros. The sequence is generated
 *            from the parity of * binomial(3k, k) which is odd iff (k &
 *            ~(3k)) == 0
 * Range:     u8 n <= 53, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed run lengths */
    static seq_t *cache = NULL;   /* array of run lengths */
    static size_t cache_size = 0; /* number of run lengths stored */
    static size_t cache_cap  = 0; /* current capacity of cache */

    /* state of the zero run generator */
    static idx_t last_k   = 0; /* smallest k not yet processed */
    static seq_t cur_run  = 0; /* length of the run currently being built */

    /* Grow cache until it contains at least n+1 run lengths. */
    while (cache_size <= (size_t)n) {
        /* parity of binomial(3k, k): 1 if odd, 0 if even */
        bool is_one = ((last_k & ~(3 * last_k)) == 0);

        if (is_one) {          /* a 1 terminates the current zero run */
            if (cur_run > 0) {
                /* store finished run */
                if (cache_size == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = cur_run;
                cur_run = 0;
            }
        } else {               /* a 0 extends the current zero run */
            cur_run++;
        }
        last_k++;               /* move to next k */
    }

    return cache[(size_t)n];
}
