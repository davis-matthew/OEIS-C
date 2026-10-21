/*
 * A216886  Primes p such that x^59 = 2 has a solution mod p.
 * https://oeis.org/A216886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, powmod64, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216886: primes p such that the congruence x^59 ≡ 2 (mod p)
   has a solution.  The function a(n) returns the nth such prime (1‑based).  */

/* Required utilities */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/powmod64.h"
#include "../utils/seq_alloc_new_array.h"

/*--------------------------------------------------------------------------*/
/* Return the nth term of the sequence.  All values are stored in a static
   cache so that calls in arbitrary order are fast.  The function works
   for all supported builds (fixed‑width integers or arbitrary‑precision)
   because it never casts an integer into seq_t directly; it only casts
   when storing a 64‑bit prime into the array and when passing arguments
   to powmod64, which returns a 64‑bit value.  The returned seq_t is
   constructed from that value by a simple cast, which is valid for
   both integer and big‑integer build types.  */
/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static state for memoised cache */
    static seq_t *seq_values = NULL;   /* array of found primes */
    static size_t seq_len   = 0;       /* number of primes stored */
    static size_t seq_cap   = 0;       /* allocated capacity of array */
    static uint64_t last_prime = 1;    /* prime to start the next search from */

    if (n == 0)
        return 0;        /* undefined but harmless */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime(last_prime);   /* next prime > last_prime   */
        last_prime = p;                        /* update for next call      */

        bool include;
        if (p == 2) {
            include = true;                     /* 2 is always included */
        } else {
            if ((p - 1) % 59 != 0) {
                include = true;                 /* any prime not ≡1 (mod59) */
            } else {
                /* by Fermat's little theorem the congruence x^59≡2 has a
                   solution iff 2^((p-1)/59) ≡ 1 (mod p)                */
                uint64_t exp = (p - 1) / 59;
                uint64_t val = powmod64(2, exp, p);
                include = (val == 1);
            }
        }

        if (include) {
            /* Ensure enough space in the array */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                seq_t *new_arr = seq_alloc_new_array(new_cap);
                if (seq_values) {
                    /* copy existing contents */
                    for (size_t i = 0; i < seq_len; ++i)
                        new_arr[i] = seq_values[i];
                    free(seq_values);
                }
                seq_values = new_arr;
                seq_cap    = new_cap;
            }
            /* Store the prime (convert to seq_t) */
            seq_values[seq_len++] = (seq_t)p;
        }
    }

    return seq_values[n - 1];
}
