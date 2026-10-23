/*
 * A136341  Fibonacci primes or semiprimes F(k) such that F(k+1) is again
 *          prime or semiprime.
 * https://oeis.org/A136341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = F(k) where F(k) is prime or semiprime and F(k+1) is also
 *            prime or semiprime. This function searches Fibonacci numbers
 *            fib(k) and fib(k+1) successively until n terms satisfying the
 *            condition are found, caching results for future calls
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   count_prime_factors, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* state kept between calls */
    static seq_t f_prev = (seq_t)1;   /* F(1)  */
    static seq_t f_curr = (seq_t)1;   /* F(2)  */
    static seq_t *terms     = NULL;   /* cached terms */
    static size_t  terms_cap = 0;     /* capacity of terms array */
    static size_t  terms_cnt = 0;     /* number of terms stored */

    /* ensure terms array is allocated */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* generate terms until we have n of them */
    while (terms_cnt < (size_t)n) {
        /* next Fibonacci number: fib(k+1) = F(k-1)+F(k) */
        seq_t f_next = f_prev + f_curr;

        /* check if F(k)=f_curr is prime or 2-almost prime */
        bool curr_ok = is_prime_big(f_curr);
        if (!curr_ok) {
            curr_ok = (count_prime_factors(f_curr) == (seq_t)2);
        }

        /* check if F(k+1)=f_next is prime or 2-almost prime */
        bool next_ok = is_prime_big(f_next);
        if (!next_ok) {
            next_ok = (count_prime_factors(f_next) == (seq_t)2);
        }

        /* if both are good, store the term */
        if (curr_ok && next_ok) {
            if (terms_cnt >= terms_cap) {
                terms_cap <<= 1;                /* double capacity */
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = f_curr;
        }

        /* advance Fibonacci window: shift left */
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* return the requested term (1‑based index) */
    return terms[n - 1];
}
