/*
 * A236575  Number of primes between successive numbers that are not
 *          squarefree.
 * https://oeis.org/A236575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 871, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the OEIS sequence A236575.
 * a(n) = number of primes strictly between the n‑th and (n+1)‑st nonsquarefree integers.
 * The sequence is built lazily: each call ensures enough nonsquarefree numbers are found
 * before returning the required term.  No expansion of the result is performed.
 */

#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* the offset is 1, but guard for safety */
        return 0;

    /* static state */
    static idx_t nf_count      = 0;           /* number of nonsquarefree numbers found so far */
    static idx_t *nf_arr       = NULL;       /* array of those numbers (idx_t)            */
    static size_t nf_cap       = 0;           /* capacity of nf_arr                         */
    static seq_t *a_arr        = NULL;       /* computed a values (prime counts)           */
    static size_t a_cap        = 0;           /* capacity of a_arr                          */
    static idx_t current       = 1;           /* last integer examined */
    static bool initialized    = false;      /* whether the static pointers have been set */

    /* initialize static pointers once */
    if (!initialized) {
        nf_arr = NULL;
        a_arr  = NULL;
        initialized = true;
    }

    /* Ensure enough nonsquarefree numbers are generated
       (we need n+1 to compute a(n), because a(n) uses the
        interval between the n‑th and (n+1)‑st nonsquarefree integers). */
    while (nf_count < n + 1) {
        /* Find the next nonsquarefree integer */
        do {
            current++;
        } while (is_squarefree((seq_t)current));

        /* Grow nf_arr if necessary */
        if (nf_cap <= nf_count) {
            size_t newcap = nf_cap ? nf_cap * 2 : 8;
            nf_arr = (idx_t *)realloc(nf_arr, newcap * sizeof(idx_t));
            nf_cap = newcap;
        }
        nf_arr[nf_count] = current;

        /* Compute a(n) for the preceding pair, if any.
           (When nf_count == 0 we are at the very first nonsquarefree number, so nothing to do.) */
        if (nf_count > 0) {
            idx_t prev = nf_arr[nf_count - 1];
            idx_t curr = nf_arr[nf_count];
            seq_t prime_count = 0;

            for (idx_t x = prev + 1; x < curr; ++x) {
                if (is_prime(x))
                    prime_count++;
            }

            /* Grow a_arr if necessary */
            if (a_cap <= nf_count - 1) {
                size_t newcap = a_cap ? a_cap * 2 : 8;
                a_arr = (seq_t *)realloc(a_arr, newcap * sizeof(seq_t));
                a_cap = newcap;
            }
            a_arr[nf_count - 1] = prime_count;
        }

        nf_count++;
    }

    /* a(n) is stored at index n-1 (since a(1) corresponds to the first pair) */
    return a_arr[n - 1];
}
