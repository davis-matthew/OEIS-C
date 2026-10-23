/*
 * A154408  Primes p such that (p^2 + 1)/10 is also prime.
 * https://oeis.org/A154408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 36, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154408: primes p such that (p^2 + 1)/10 is also prime.
   The function computes the n‑th term (1‑based) with memoization.
   It uses simple primality tests from the utility library. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – undefined for n=0 */

    static seq_t *seq = NULL;              /* cached sequence values      */
    static idx_t  seq_size = 0, seq_cap = 0;
    static idx_t  next_candidate = 3;      /* first odd prime candidate   */
    static bool    initialized = false;    /* one–time array allocation   */

    if (!initialized) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_size = 0;
        initialized = true;
    }

    while (seq_size < n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate))
            next_candidate += 2;

        /* (p^2 + 1) must be divisible by 10 for the quotient to be an integer. */
        seq_t p = (seq_t)next_candidate;
        seq_t t = p * p + 1;              /* p^2 + 1  (overflow is harmless for seq_t) */
        if ((t % (seq_t)10) != 0) {
            next_candidate += 2;
            continue;                     /* not a valid candidate */
        }

        seq_t q = t / (seq_t)10;           /* (p^2 + 1) / 10             */
        if (is_prime_big(q)) {             /* q must also be prime       */
            if (seq_size == seq_cap) {     /* grow storage if needed     */
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = next_candidate;   /* store the term           */
        }

        next_candidate += 2;               /* advance to following odd  */
    }

    return seq[n - 1];
}
