/*
 * A061759  Numbers k such that k! is divisible by (k+1)^11.
 * https://oeis.org/A061759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Tests whether k! is divisible by (k+1)^11.
   For each prime divisor p of k+1 the exponent of p in k! must be
   at least 11 times its exponent in k+1.  The routine is called
   by a(idx_t n) which returns the n‑th term of the sequence
   (a(1) is the first term). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true if k! is divisible by (k+1)^11 */
static bool is_good(idx_t k)
{
    idx_t num = k + 1;        /* value to factor */
    idx_t pp = 2;             /* current prime candidate */

    while (pp <= num / pp) {          /* trial division up to sqrt(num) */
        if (num % pp == 0) {
            uint64_t exp_kplus = 0;    /* exponent of pp in k+1 */
            while (num % pp == 0) {
                num   /= pp;
                exp_kplus++;
            }

            /* exponent of pp in k! using Legendre's formula */
            idx_t t = k;
            uint64_t exp_fact = 0;
            while (t) {
                t      /= pp;
                exp_fact += t;
            }

            if (exp_fact < 11ULL * exp_kplus)
                return false;
        }
        /* increment pp: 2 then odd numbers */
        pp = (pp == 2) ? 3 : pp + 2;
    }

    /* Remaining num > 1 is a prime divisor > sqrt(k+1) */
    if (num > 1) {
        idx_t p = num;
        idx_t t = k;
        uint64_t exp_fact = 0;
        while (t) {
            t      /= p;
            exp_fact += t;
        }
        /* exponent in k+1 is 1, so require at least 11 in k! */
        if (exp_fact < 11ULL)
            return false;
    }

    return true;
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cached terms   */
    static size_t cap   = 0;     /* allocated size */
    static idx_t  len   = 0;     /* number of cached terms */
    static idx_t  last_k = 0;    /* last k examined   */

    if (n == 0)
        return (seq_t)0;         /* undefined below offset */

    /* Generate and cache terms until we have at least n terms. */
    while (len < n) {
        last_k++;

        if (is_good(last_k)) {
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort the program. */
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
