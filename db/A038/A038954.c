/*
 * A038954  Primes p such that 70 is not a square mod p.
 * https://oeis.org/A038954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the primes p for which 70 is a quadratic
   non-residue modulo p.  In terms of Jacobi symbols this means
   (70|p) = –1.  The implementation enumerates primes, checks the
   Jacobi symbol and memoises the qualifying primes in a static array. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

/* Memoised list of qualifying primes.  The array is dynamically
   grown as needed. */
static seq_t *qual = NULL;     /* ptr to array of seq_t */
static size_t qlen = 0;        /* number of stored primes          */
static size_t qcap = 0;        /* current capacity of the array    */

/* Next odd integer to test for primality; starts at 3.  Only odd
   values are stored because 2 is excluded from the sequence. */
static uint64_t next_candidate = 3;

/* Return the nth prime p such that 70 is not a quadratic residue mod p
   (n ≥ 1).  The function is safe to call with n out of order; the list of
   qualifying primes is memoised. */
seq_t a(idx_t n)
{
    /* Handle the offset (the sequence is defined for n ≥ 1 only). */
    if (n == 0) return 0;

    /* Grow the memoised list until it contains at least n terms. */
    while (qlen < n) {
        uint64_t p = next_candidate;

        /* Find the next prime candidate.  Since we only ever test odd
           numbers, we add 2 each time.  The primality test is provided
           by the framework. */
        while (!is_prime(p)) {
            p += 2;
        }

        /* Advance the starting point for the next search to be
           the next odd integer after the current prime. */
        next_candidate = p + 2;

        /* Skip the primes that divide 70; for them (70|p) = 0. */
        if (p == 5 || p == 7) {
            continue;
        }

        /* If 70 is a quadratic non-residue modulo p, store p. */
        if (jacobi_symbol(70ULL, p) == -1) {
            /* Grow the array if necessary. */
            if (qlen == qcap) {
                size_t newcap = qcap ? qcap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(qual, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort gracefully. */
                    exit(1);
                }
                qual = tmp;
                qcap = newcap;
            }
            qual[qlen++] = (seq_t)p;
        }
    }

    /* Return the requested term (1-based indexing). */
    return qual[n - 1];
}
