/*
 * A184902  Primes that are not factors of m^2 + m + 11 (A048058).
 * https://oeis.org/A184902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3268, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_in_residue, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes that are not factors of m^2 + m + 11 (A184902)
   A prime p is in the sequence iff (−43|p) = −1, i.e.  
   p % 43 ∈ {2, 3, 5, 7, 8, 12, 18, 19, 20, 22, 26, 27, 28,
              29, 30, 32, 33, 34, 37, 39, 42}.  The function returns
   the n‑th such prime with a(1)=2. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_in_residue.h"

/*------------------------------------------------------------------
 *  Cache of admissible primes and state of the prime generator.
 *------------------------------------------------------------------*/
static seq_t *admissible_primes = NULL;   /* array of primes */
static size_t admissible_count = 0;       /* number stored */
static size_t admissible_cap   = 0;       /* allocated capacity */
static uint64_t last_checked   = 1ULL;    /* last integer examined */

/*------------------------------------------------------------------
 *  Ensure that at least n admissible primes have been generated.
 *------------------------------------------------------------------*/
static void ensure_n(idx_t n)
{
    if (admissible_count >= n)
        return;

    /* Allocate initial capacity if needed */
    if (admissible_cap == 0) {
        admissible_cap = 64;
        admissible_primes = (seq_t *)malloc(admissible_cap * sizeof(seq_t));
    }

    /* Generate primes until we have n of them */
    while (admissible_count < n) {
        last_checked++;                     /* consider next integer */
        if (!is_prime(last_checked))
            continue;                       /* skip composites     */
        if (!is_in_residue(last_checked))
            continue;                       /* skip primes with
                                                discriminant a square */

        /* Store prime */
        if (admissible_count == admissible_cap) {
            admissible_cap *= 2;
            admissible_primes =
                (seq_t *)realloc(admissible_primes,
                                 admissible_cap * sizeof(seq_t));
        }
        admissible_primes[admissible_count++] = (seq_t)last_checked;
    }
}

/*------------------------------------------------------------------
 *  Public sequence function.
 *------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) is undefined */

    ensure_n(n);
    return admissible_primes[n - 1];
}
