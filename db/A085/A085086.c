/*
 * A085086  a(1) = 1; if n is composite then a(n) = Sum_{i < n, i not prime}
 *          a(i), else if n is prime then a(n) = sum_{ j < n, j is a
 *          noncomposite} a(j).
 * https://oeis.org/A085086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1;
   for n > 1
     if n is prime    :  a(n) = a(1) + Σ_{p < n} a(p)   (sum over primes less than n)
     else (composite) :  a(n) = a(1) + Σ_{c < n} a(c)   (sum over composites less than n)
   (1 is considered neither prime nor composite, but it is counted through a(1).) */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static data used to memoize values and keep running sums */
    static seq_t *a_cache     = NULL;   /* a_cache[1..cached_max] */
    static idx_t cached_max   = 0;      /* largest n that has been computed */

    static seq_t prime_sum   = 0;      /* Σ a(p) for all primes p < current n */
    static seq_t comp_sum    = 0;      /* Σ a(c) for all composites c < current n */

    static idx_t *prime_list = NULL;   /* list of primes found so far */
    static size_t prime_cnt  = 0;
    static size_t prime_cap  = 0;

    /* initialise for n = 1 */
    if (cached_max == 0) {
        a_cache     = (seq_t*)malloc(2 * sizeof(seq_t)); /* indices 0 and 1 */
        a_cache[1]  = (seq_t)1;
        cached_max  = 1;
        prime_sum   = 0;
        comp_sum    = 0;
    }

    /* extend cache to include all indices up to n */
    if (n > cached_max) {
        size_t old_max = (size_t)cached_max;
        size_t new_sz  = (size_t)n + 1; /* need indices 0..n */

        a_cache = (seq_t*)realloc(a_cache, new_sz * sizeof(seq_t));

        for (idx_t j = old_max + 1; j <= n; ++j) {
            /* test primality of j using the primes found so far */
            bool is_prime = (j >= 2);
            for (size_t k = 0; k < prime_cnt; ++k) {
                idx_t p = prime_list[k];
                if (p * p > j) break;
                if (j % p == 0) { is_prime = false; break; }
            }

            seq_t val;
            if (is_prime) {
                /* prime case: a(j) = a(1) + prime_sum */
                val = a_cache[1] + prime_sum;
                a_cache[j] = val;
                prime_sum += val;

                /* append j to prime list */
                if (prime_cnt == prime_cap) {
                    size_t new_cap = (prime_cap == 0) ? 4 : prime_cap * 2;
                    prime_list = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
                    prime_cap  = new_cap;
                }
                prime_list[prime_cnt++] = j;
            } else {
                /* composite (or 1) case: a(j) = a(1) + comp_sum */
                val = a_cache[1] + comp_sum;
                a_cache[j] = val;
                comp_sum  += val;
                /* 1 is never added to prime_list; it is already accounted for via a_cache[1]. */
            }
        }

        cached_max = n;
    }

    return a_cache[n];
}
