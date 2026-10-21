/*
 * A068419  Odd prime values of sigma(k) - phi(k) taking k in increasing
 *          order.
 * https://oeis.org/A068419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th odd prime value of sigma(k) – phi(k) as k runs
 *            over * positive integers. The values are found by brute‑force
 *            search
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq_array, factor_uint64, is_prime_big, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on a few utilities from the OEIS helper library: */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Utility for array allocation and freeing for seq_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) -> seq_t*  */
#include "../utils/seq_free_array.h"    /* seq_free_array(seq_t*) */

/* Utility to factor a uint64_t */
#include "../utils/factor_uint64.h"     /* void factor_uint64(uint64_t,x,fac,exp,&cnt) */

/* Prime test for arbitrary‑precision seq_t values */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* Static state to cache found terms between calls.  No assumption that
     * calls occur in increasing order. */
    static seq_t *vals = NULL;   /* array of collected terms */
    static size_t vals_len = 0;  /* number of terms stored */
    static size_t vals_cap = 0;  /* current allocation size */
    static uint64_t next_k = 1;  /* next k to test, starts at 1 */

    /* initialise static data on first invocation */
    if (vals == NULL) {
        vals_cap = 16;                          /* initial arbitrary size */
        vals = alloc_seq_array(vals_cap);        /* array of seq_t */
        vals_len = 0;
        next_k = 1;
    }

    /* generate terms until we have at least n of them */
    while (vals_len < (size_t)n) {
        uint64_t k = next_k++;          /* candidate k */

        /* Factor k */
        uint64_t fac[20] = {0};
        int      exp[20] = {0};
        int      cnt     = 0;
        factor_uint64(k, fac, exp, &cnt);

        /* Compute sigma(k) */
        seq_t sigma = (seq_t)1;
        for (int i = 0; i < cnt; ++i) {
            seq_t term = (seq_t)1;
            seq_t pp   = (seq_t)1;
            for (int j = 0; j < exp[i]; ++j) {
                pp *= (seq_t)fac[i];
                term += pp;
            }
            sigma *= term;
        }

        /* Compute phi(k) = prod p_i^(e_i-1) * (p_i-1) */
        seq_t phi = (seq_t)1;
        for (int i = 0; i < cnt; ++i) {
            seq_t part = (seq_t)(fac[i] - 1);
            for (int j = 1; j < exp[i]; ++j) {
                part *= (seq_t)fac[i];
            }
            phi *= part;
        }

        /* diff = sigma - phi */
        seq_t diff = sigma - phi;

        /* Keep only odd primes > 1 */
        if ((diff & (seq_t)1) && diff > (seq_t)1 && is_prime_big(diff)) {
            /* Grow array if necessary */
            if (vals_len == vals_cap) {
                size_t new_cap = vals_cap * 2;
                seq_t *new_vals = alloc_seq_array(new_cap);
                memcpy(new_vals, vals, vals_len * sizeof(seq_t));
                seq_free_array(vals);
                vals = new_vals;
                vals_cap = new_cap;
            }
            vals[vals_len++] = diff;
        }
    }

    return vals[n - 1];
}
