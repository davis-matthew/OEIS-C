/*
 * A199821  Primes of form 12^n-11^n.
 * https://oeis.org/A199821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 12^k – 11^k for k ≥ 2. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static state for out‑of‑order calls */
    static seq_t *primes      = NULL;
    static size_t primes_cnt  = 0;
    static size_t primes_cap  = 0;
    static idx_t  curr_k      = 2;          /* exponent currently examined   */
    static seq_t  pow12        = 0;          /* 12^curr_k                      */
    static seq_t  pow11        = 0;          /* 11^curr_k                      */
    static bool   initialized = false;

    /* Initialise the first powers for k = 2 (12² and 11²). */
    if (!initialized) {
        pow12 = (seq_t)12 * (seq_t)12;       /* 12²  = 144                     */
        pow11 = (seq_t)11 * (seq_t)11;       /* 11²  = 121                     */
        initialized = true;
    }

    /* Generate primes until at least n of them are found. */
    while (primes_cnt < n) {
        seq_t candidate = pow12 - pow11;     /* 12^k – 11^k                    */

        if (is_prime_big(candidate)) {
            /* Grow the output array if necessary. */
            if (primes_cnt == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 8 : primes_cap * 2;
                primes = seq_realloc(primes, primes_cap, new_cap);
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }

        /* Advance to the next exponent. */
        ++curr_k;
        pow12 *= (seq_t)12;
        pow11 *= (seq_t)11;
    }

    return primes[n - 1];
}
