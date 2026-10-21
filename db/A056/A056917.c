/*
 * A056917  Numbers n such that sigma(n)-phi(n) is a perfect square.
 * https://oeis.org/A056917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th integer m such that sigma(m) - phi(m) is a perfect
 *            square
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a lazy sieve: maintain a dynamic array of found values
   and extend it until the requested index is reached.  The helper functions
   from "utils" are used for sigma, phi and integer square root. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n) */
#include "../utils/calc_phi.h"   /* seq_t calc_phi(idx_t n) */
#include "../utils/floor_sqrt.h"/* seq_t floor_sqrt(seq_t x) */

/* dynamic array to cache the sequence values */
static seq_t *seq = NULL;   /* array of seq_t */
static idx_t  seq_len = 0;  /* number of values currently stored */
static idx_t  seq_cap = 0;  /* allocated capacity */

/* ensure that the array can hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t newcap = seq_cap > 0 ? seq_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* append a new value to the cached array */
static void append_value(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq[seq_len++] = val;
}

/* test whether n satisfies the defining property */
static bool property_holds(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t phi   = calc_phi(n);
    seq_t diff  = sigma - phi;
    seq_t root  = floor_sqrt(diff);
    return (root * root) == diff;
}

/* public accessor for the n-th element of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1, return zero safely */

    static idx_t curr = 1; /* next integer to test */

    while (seq_len < n) {
        if (property_holds(curr))
            append_value(curr);
        ++curr;
    }
    return seq[n - 1];
}
