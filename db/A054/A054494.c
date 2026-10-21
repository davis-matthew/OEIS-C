/*
 * A054494  Largest Fibonacci factor of n.
 * https://oeis.org/A054494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest Fibonacci divisor of n (so a(10)=5 because 5 is
 *            the largest Fibonacci number dividing 10)
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest Fibonacci divisor of n */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    /* static cache of Fibonacci numbers */
    static seq_t *fib = NULL;
    static size_t fib_cnt = 0;   /* number of Fibonacci numbers stored */
    static size_t fib_cap = 0;   /* capacity of the array */

    /* convert the index to the seq_t type for computations */
    seq_t n_seq = (seq_t)n;

    /* initialise cache on first call */
    if (fib == NULL) {
        fib_cap = 4;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (fib == NULL) return 0;          /* allocation failure */
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)1;
        fib_cnt = 2;
    }

    /* grow the Fibonacci table so that all fib <= n_seq are present */
    while (fib_cnt > 1 && fib[fib_cnt - 1] <= n_seq) {
        seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        if (next > n_seq) break;            /* we've reached beyond n */

        if (fib_cnt == fib_cap) {           /* double capacity if needed */
            size_t new_cap = fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;      /* allocation failure */
            fib = tmp;
            fib_cap = new_cap;
        }
        fib[fib_cnt++] = next;
    }

    /* search from largest to smallest for a divisor */
    for (size_t i = fib_cnt; i-- > 0;) {
        if (n_seq % fib[i] == 0) {
            return fib[i];
        }
    }

    /* the number 1 always divides n, so we never fall through */
    return n_seq;
}
