/*
 * A089828  Number of subsets of {1,.., n} containing at least one twin prime
 *          pair.
 * https://oeis.org/A089828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - (number of subsets of {1..n} that contain no twin
 *            prime pair)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes_upto, fib_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The complement is computed by treating the primes up to n as chains of consecutive
   twin primes.  For a chain of k primes the number of admissible selections is the
   Fibonacci number F_{k+2}.  The total number of subsets without a twin pair is
   (∏_chains F_{k+2}) · 2^(n‑π(n)), where π(n) is the number of primes ≤ n.  */

/* -------------------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"  /* build_primes_upto(limit,&primes,&cnt) */
#include "../utils/pow2.h"                /* pow2(exp)       */
#include "../utils/fib_small.h"
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) { /* not required by OEIS but keeps the function well‑defined */
        return (seq_t)0;
    }

    /* Build list of primes ≤ n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Count subsets that contain no twin prime pair */
    seq_t prod_fib = (seq_t)1;          /* product over chains      */
    idx_t i = 0;

    while (i < (idx_t)prime_cnt) {
        /* Determine the length k of the current chain of consecutive twins */
        idx_t chain_len = 1;
        while ( (i + 1 < (idx_t)prime_cnt) &&
                (idx_t)(primes[i + 1]) - (idx_t)(primes[i]) == (idx_t)2) {
            ++chain_len;
            ++i;
        }

        /* Multiply by F_{k+2} for this chain */
        prod_fib *= fib_small(chain_len + (idx_t)2);

        ++i; /* move to the next prime after the chain */
    }

    /* Subsets of the non‑prime elements (and isolated primes are already handled
       by the Fibonacci factor) */
    seq_t pow2_nonprime = pow2(n - (idx_t)prime_cnt);

    /* Total subsets of {1..n} that avoid a twin prime pair */
    seq_t no_twin = prod_fib * pow2_nonprime;

    /* Total subsets of {1..n} */
    seq_t all_subsets = pow2(n);

    /* Desired sequence value: subsets that contain at least one twin prime pair */
    seq_t result = all_subsets - no_twin;

    /* Clean up dynamically allocated prime array */
    free(primes);

    return result;
}
