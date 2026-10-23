/*
 * A134069  Primes in A038399.
 * https://oeis.org/A134069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  bref, hard, more, nonn, base
 * Formula:   a(n) = nth prime occurring in A038399(n) (concatenation of the
 *            first n non‑zero Fibonacci numbers in reverse order)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *prime_cache   = NULL;
    static size_t  cache_cap    = 0;
    static size_t  cache_size   = 0;

    /* state of the concatenation for A038399 */
    static idx_t    cur_k        = 1;          /* current n in A038399 (starts at 1) */
    static seq_t    cur_val      = (seq_t)1;   /* A038399(cur_k) */
    static seq_t    fib_prev    = (seq_t)1;   /* fib(cur_k)   (initially fib(1)) */
    static seq_t    fib_next    = (seq_t)1;   /* fib(cur_k+1) (initially fib(2)) */

    /* initialise cache lazily */
    if (prime_cache == NULL) {
        /* nothing to do – static vars already start with sent values */
    }

    /* generate more terms until we have at least n primes */
    while (cache_size < n) {
        /* build next A038399 term: fib_next * 10^digits(cur_val) + cur_val */
        int    digits = digit_len(cur_val);
        seq_t  p10    = pow10((idx_t)digits);
        seq_t  next_val = fib_next * p10 + cur_val;

        /* test for primality */
        if (is_prime_big(next_val)) {
            /* grow cache if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;                      /* out of memory – unlikely in test harness */
                for (size_t i = 0; i < cache_size; ++i) tmp[i] = prime_cache[i];
                free(prime_cache);
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_size++] = next_val;
        }

        /* advance Fibonacci numbers for next iteration */
        seq_t new_fib = fib_prev + fib_next;
        fib_prev = fib_next;
        fib_next = new_fib;

        /* advance to next A038399 index */
        cur_k++;
        cur_val = next_val;
    }

    /* return the requested prime (1‑based index) */
    return prime_cache[(size_t)n - 1];
}
