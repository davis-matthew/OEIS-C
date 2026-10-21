/*
 * A059109  Numbers m such that m*phi(m)-1 is prime, where phi is the Euler
 *          function (A000010).
 * https://oeis.org/A059109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that m*phi(m)-1 is prime
 * Range:     u8 n <= 11, u16 n <= 285, u32 n <= 63, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"     /* declares calc_phi(seq_t)          */
#include "../utils/is_prime_big.h" /* declares is_prime_big(seq_t)      */
#include "../utils/seq_mul.h"      /* declares seq_mul(seq_t, seq_t)   */

static seq_t *cache = NULL; /* stored values                         */
static size_t cap   = 0;    /* capacity of cache                    */
static size_t sz    = 0;    /* number of values stored              */
static idx_t tested_m = 2;  /* last m value that was examined       */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cap < need) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }
}

/* Append a value to the cache, expanding if necessary. */
static void append_value(seq_t val)
{
    ensure_capacity(sz + 1);
    cache[sz++] = val;
}

/* Test whether m satisfies m*phi(m)-1 is prime.  */
static bool good_m(idx_t m)
{
    seq_t sm     = (seq_t)m;           /* m as seq_t */
    seq_t phi_m  = calc_phi(sm);
    seq_t prod   = seq_mul(sm, phi_m); /* m * phi(m) */
    seq_t n      = prod - (seq_t)1;    /* m*phi(m) - 1 */
    return is_prime_big(n);
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1, return 0 */

    while (sz < (size_t)n) {
        /* Search for the next good m */
        while (true) {
            ++tested_m;
            if (good_m(tested_m)) {
                append_value((seq_t)tested_m);
                break;
            }
        }
    }
    return cache[n - 1];
}
