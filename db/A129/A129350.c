/*
 * A129350  The smallest number in a group of four consecutive mutually
 *          coprime integers such that the sum of three of them plus the
 *          square of the fourth is prime.
 * https://oeis.org/A129350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 449, u32 n <= 60, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A129350: for each a, find the smallest coprime
   b > a, c > b, d > c such that all four are pairwise coprime.
   If any of the sums a²+b+c+d, a+b²+c+d, a+b+c²+d, a+b+c+d² is prime,
   a is a term of the sequence. */

/* Necessary utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

/* Memoisation storage */
static seq_t *cache_vals = NULL;   /* cache of computed a-values */
static size_t cache_len = 0;       /* number of stored values */
static size_t cache_cap = 0;       /* allocated capacity of cache_vals */
static idx_t  next_candidate = 1; /* next a to examine when expanding cache */

/* Test coprimality of two seq_t values */
static inline bool coprime(seq_t x, seq_t y)
{
    return gcd(x, y) == ((seq_t)1);
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined for n==0 – return 0 */
        return (seq_t)0;

    /* Grow the cache until we have at least n entries */
    while (cache_len < n) {
        /* --- Build the quadruple for current a = next_candidate --- */
        idx_t a_val = next_candidate;

        /* Find smallest b coprime to a */
        idx_t b_val = a_val + (idx_t)1;
        while (!coprime((seq_t)a_val, (seq_t)b_val))
            b_val++;

        /* Find smallest c coprime to a and b */
        idx_t c_val = b_val + (idx_t)1;
        while (!(coprime((seq_t)a_val, (seq_t)c_val) &&
                 coprime((seq_t)b_val, (seq_t)c_val)))
            c_val++;

        /* Find smallest d coprime to a, b and c */
        idx_t d_val = c_val + (idx_t)1;
        while (!(coprime((seq_t)a_val, (seq_t)d_val) &&
                 coprime((seq_t)b_val, (seq_t)d_val) &&
                 coprime((seq_t)c_val, (seq_t)d_val)))
            d_val++;

        /* Convert to seq_t for arithmetic */
        seq_t sa = (seq_t)a_val;
        seq_t sb = (seq_t)b_val;
        seq_t sc = (seq_t)c_val;
        seq_t sd = (seq_t)d_val;

        /* Test the four possible prime sums */
        bool ok = false;
        if (is_prime_big(sa * sa + sb + sc + sd))
            ok = true;
        else if (is_prime_big(sa + sb * sb + sc + sd))
            ok = true;
        else if (is_prime_big(sa + sb + sc * sc + sd))
            ok = true;
        else if (is_prime_big(sa + sb + sc + sd * sd))
            ok = true;

        /* If a qualifies, store it in the cache */
        if (ok) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : (size_t)8);
                seq_t *tmp = (seq_t *)realloc(cache_vals,
                                              new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – abort */
                    return (seq_t)0;
                cache_vals = tmp;
                cache_cap = new_cap;
            }
            cache_vals[cache_len++] = sa;
        }

        /* Move to next a candidate */
        next_candidate++;
    }

    /* Return the n-th term (1‑based) */
    return cache_vals[n - (idx_t)1];
}
