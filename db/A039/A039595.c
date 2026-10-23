/*
 * A039595  Positive Fibonacci numbers that are also triangular numbers.
 * https://oeis.org/A039595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n‑th Fibonacci number F_k such that 8*F_k+1 is a
 *            perfect square (i.e. F_k is a triangular number). The known
 *            terms are 1, 3, 21, 55 and the sequence is proved to have no
 *            further terms. This routine generates Fibonacci numbers on
 *            demand, checks the triangular test via floor_sqrt, and c...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Static cache for the found terms.  We anticipate far fewer than a thousand
   entries; 1000 gives ample room for any future extensions. */
static seq_t cache[1000];
static idx_t cache_len = 0;

/* State for the Fibonacci generation.  Starting from F_0 = 0, F_1 = 1. */
static seq_t f_prev = 0;
static seq_t f_curr = 1;
static idx_t fib_index = 1;   /* fib_index == index of f_curr (F_1) */

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate Fibonacci numbers until we have found the requested term
       or until we have scanned past a reasonable bound (1000 indices). */
    while (cache_len < n && fib_index < 1000) {
        /* Next Fibonacci number: F_{fib_index+1} = f_prev + f_curr */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++fib_index;

        /* Check if f_curr is triangular: 8*f_curr + 1 must be a square. */
        seq_t test = 8 * f_curr + 1;
        seq_t s = floor_sqrt(test);
        if (s * s == test) {
            if (cache_len < sizeof(cache) / sizeof(cache[0]))
                cache[cache_len++] = f_curr;
            else
                /* Cache overflow – sequence is longer than preallocated
                   space; beyond this we cannot provide further terms. */
                return 0;
        }
    }

    /* If we reached the required term, return it; otherwise, no such term. */
    return (n <= cache_len) ? cache[n - 1] : (seq_t)0;
}
