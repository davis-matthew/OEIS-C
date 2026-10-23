/*
 * A130255  Maximal index k of an odd Fibonacci number (A001519) such that
 *          A001519(k) = Fibonacci(2k-1) <= n (the 'lower' odd Fibonacci
 *          Inverse).
 * https://oeis.org/A130255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + log_phi(sqrt(5)*n))/2)
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts the odd Fibonacci numbers
 * (A001519) not greater than n by generating them on demand.
 * It keeps a static cache of values F(1),F(3),F(5),… .
 * When the next odd‑index Fibonacci exceeds n the loop stops,
 * and the number of stored values is a(n). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Static cache of odd‑index Fibonacci numbers (F(1), F(3), F(5), …) */
    static seq_t *odd_fib = NULL;   /* array of stored Fibonacci values */
    static idx_t odd_fib_cnt = 0;   /* number of values currently stored */
    static idx_t odd_fib_cap = 0;   /* current capacity of the array */
    static idx_t next_odd_index = 1;/* next odd index to generate */

    seq_t target = n;              /* cast n to seq_t for comparison */

    /* Grow the cache until the next odd Fibonacci would be > n */
    while (1)
    {
        seq_t val = fib(next_odd_index);   /* Fibonacci at odd index */
        if (val > target)                  /* stop once we exceed n    */
            break;

        /* Resize the array if needed */
        if (odd_fib_cnt == odd_fib_cap)
        {
            idx_t new_cap = odd_fib_cap ? odd_fib_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(odd_fib,
                (size_t)new_cap * sizeof(seq_t));
            if (!tmp) abort();             /* allocation failure */
            odd_fib = tmp;
            odd_fib_cap = new_cap;
        }

        odd_fib[odd_fib_cnt++] = val;      /* store the value      */
        next_odd_index += 2;               /* advance to next odd index (3,5,7,…) */
    }

    return (seq_t)odd_fib_cnt;             /* a(n) is the count of stored values */
}
