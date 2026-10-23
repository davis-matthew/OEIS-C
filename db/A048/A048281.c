/*
 * A048281  Beginning of first run of consecutive quadratic residues mod
 *          prime(n) of longest length.
 * https://oeis.org/A048281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements the definition of A048281: 
   For n ≥ 1 let p = prime(n).  In the infinite set of integers, 
   an integer is a quadratic residue modulo p if its remainder mod p 
   is a quadratic residue.  Among all maximal consecutive runs of such
   residues, the function returns the smallest starting integer.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Cached list of primes */
static uint64_t *primes = NULL;   /* primes[0] == prime(1) */
static size_t   primes_cnt = 0;   /* number of primes cached so far */

/* Ensure that the first n primes are available in the cache. */
static void ensurePrimes(idx_t n)
{
    if (n <= (idx_t)primes_cnt)
        return;   /* already enough */

    /* Find a limit that guarantees at least n primes. */
    idx_t limit = _prime_limit_for_idx(n);
    /* Re‑allocate a new sieve and store it. */
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Replace old cache with the new one. */
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
}

/* Return the starting integer of the first longest run of consecutive
   quadratic residues modulo prime(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but return 0 */

    /* Get prime(n). */
    ensurePrimes(n);
    uint64_t p = primes[n - 1];
    if (p == 0)
        return (seq_t)0;   /* safety guard */

    /* Allocate a mask for the residues modulo p. */
    size_t psize = (size_t)p;
    uint8_t *mask = (uint8_t *)calloc(psize, 1);
    if (!mask)
        return (seq_t)0;   /* allocation failure */

    /* Mark quadratic residues: x^2 mod p. */
    for (uint64_t x = 0; x < p; ++x) {
        uint64_t r = (x * x) % p;
        mask[r] = 1;
    }

    /* Scan the doubled mask to handle wrap‑around runs. */
    size_t best_len   = 0;
    size_t best_start = 0;

    size_t cur_len   = 0;
    size_t cur_start = 0;

    for (size_t i = 0; i < 2 * psize; ++i) {
        if (mask[i % psize]) {
            if (cur_len == 0)
                cur_start = i % psize;
            ++cur_len;
        } else {
            if (cur_len > best_len) {
                best_len   = cur_len;
                best_start = cur_start % psize;
            }
            cur_len = 0;
        }
    }
    /* Final run may reach the end of the doubled array. */
    if (cur_len > best_len) {
        best_len   = cur_len;
        best_start = cur_start % psize;
    }

    /* If the run length exceeds the period, all numbers are residues
       and the earliest run starts at 0. */
    if (best_len > psize)
        best_start = 0;

    free(mask);
    return (seq_t)best_start;
}
