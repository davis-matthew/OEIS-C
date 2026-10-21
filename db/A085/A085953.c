/*
 * A085953  a(1) = 2, a(n+1) = the smallest multiple of prime(n+1) beginning
 *          with a(n).
 * https://oeis.org/A085953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; a(n+1) = the smallest multiple of prime(n+1) that
 *            begins with a(n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"     /* sieve_first_n_primes() */
#include "../utils/alloc_seq_array.h"          /* alloc_seq_array()   */

/*------------------------------------------------------------*/
/*  Return the nth term of OEIS A085953.
    The function is memoised so calls in any order are fast.   */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static uint32_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, … */
    static size_t prim_cnt   = 0;
    static seq_t *seq_arr    = NULL;      /* seq_arr[0] unused, seq_arr[1] = 2 */
    static size_t arr_sz     = 0;
    static size_t last_used  = 0;         /* largest index currently filled */

    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* 1. Ensure we have at least n primes -------------------------------- */
    if (prim_cnt < (size_t)n) {
        free(primes);
        primes = NULL;
        sieve_first_n_primes((idx_t)n, &primes, &prim_cnt);
    }

    /* 2. Allocate or grow the sequence array -------------------------------- */
    if ((size_t)n > arr_sz) {
        seq_t *new_arr = (seq_t *)alloc_seq_array((size_t)(n + 1));
        if (!new_arr) { return 0; }      /* allocation failure handled by caller */
        if (seq_arr) {
            for (size_t i = 1; i <= last_used; ++i)
                new_arr[i] = seq_arr[i];
            free(seq_arr);
        }
        seq_arr = new_arr;
        arr_sz  = n;
    }

    /* 3. Initialise the first term if necessary ---------------------------- */
    if (last_used == 0) {
        seq_arr[1] = (seq_t)2;
        last_used  = 1;
    }

    /* 4. Compute missing terms ------------------------------------------- */
    for (idx_t idx = (idx_t)(last_used + 1); idx <= n; ++idx) {
        seq_t prefix = seq_arr[idx - 1];          /* a(idx-1)          */
        uint32_t p   = primes[idx - 1];           /* prime(idx)        */

        /* Current remainder of prefix * 10^d (mod p).
           We start with d = 0. */
        seq_t cur_mod = prefix % p;              /* prefix · 10^0 mod p */
        seq_t pow10   = (seq_t)1;                /* 10^d as seq_t       */

        for (;;) {
            /* Residue needed to reach a multiple of p. */
            seq_t r_needed = (p - cur_mod) % p;

            /* If r_needed fits into the d‑digit suffix, we found the answer. */
            if (r_needed < pow10) {
                seq_arr[idx] = prefix * pow10 + r_needed;
                break;
            }

            /* Grow suffix by another digit.  Keep cur_mod modulo p. */
            cur_mod = (cur_mod * (seq_t)10) % p;
            pow10   = pow10 * (seq_t)10;
        }
    }

    last_used = n;
    return seq_arr[n];
}
