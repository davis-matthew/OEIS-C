/*
 * A067005  Totient of A061026(n) divided by n.
 * https://oeis.org/A067005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066678(n)/n = phi(A061026(n))/n where A061026(n) is the
 *            smallest m such that n divides phi(m)
 * Range:     u8 n <= 30, u16 n <= 7818, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"

static seq_t *a_cache = NULL;   /* cache of a(n) values, indexed by n-1 */
static idx_t  a_cache_cap = 0;  /* capacity of a_cache */

/* Compute the first m such that n divides phi(m) and return phi(m)/n */
static seq_t compute_a(idx_t n)
{
    for (idx_t m = 1; ; ++m) {
        seq_t phi_m = calc_phi((seq_t)m);          /* phi(m) */
        if ((phi_m % (seq_t)n) == 0) {
            return phi_m / (seq_t)n;              /* a(n) */
        }
    }
}

/* External function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) {           /* undefined for n=0, return 0 for safety */
        return 0;
    }

    /* Ensure cache has space for n */
    if (n > a_cache_cap) {
        size_t new_cap = a_cache_cap * 2;
        if (new_cap < n) new_cap = n;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        for (idx_t i = a_cache_cap; i < new_cap; ++i) a_cache[i] = 0;
        a_cache_cap = new_cap;
    }

    /* Return cached value if available */
    if (a_cache[n - 1] != 0) {
        return a_cache[n - 1];
    }

    /* Compute, cache, and return */
    seq_t result = compute_a(n);
    a_cache[n - 1] = result;
    return result;
}
