/*
 * A111180  Product_{k=1..n} prime(k)!.
 * https://oeis.org/A111180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=1..n} prime(k)!, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Memoization state */
    static seq_t *memo          = NULL;    /* array of a(k) values */
    static size_t cap           = 0;       /* current allocation size */
    static idx_t last_n         = 0;       /* largest n computed */
    static seq_t product        = 1;       /* product of prime(k)! up to last_n */
    static seq_t fact           = 1;       /* factorial of current prime */
    static seq_t prev_prime     = 1;       /* previous prime processed (starts at 1) */

    /* Allocate memo array on first call */
    if (memo == NULL) {
        /* Ensure at least one element for a(0) */
        cap = 1;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (memo == NULL) exit(EXIT_FAILURE);
        memo[0] = 1;          /* a(0) = 1 */
        last_n = 0;
    }

    /* Grow memo array if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        memo = tmp;
        cap  = newcap;
    }

    /* Return cached value if already computed */
    if (n <= last_n)
        return memo[(size_t)n];

    /* Compute terms up to n */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* Get k‑th prime (1‑indexed) */
        seq_t p = get_prime(k);

        /* Extend the current factorial from prev_prime+1 up to p */
        idx_t start = (idx_t)prev_prime + 1;
        idx_t end   = (idx_t)p;
        for (idx_t i = start; i <= end; ++i)
            fact *= i;

        /* Update the overall product */
        product *= fact;

        /* Store the result */
        memo[(size_t)k] = product;

        /* Update previous prime for next iteration */
        prev_prime = p;
    }

    last_n = n;
    return memo[(size_t)n];
}
