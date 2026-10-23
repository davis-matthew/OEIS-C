/*
 * A038914  Primes p such that 37 is not a square mod p.
 * https://oeis.org/A038914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 37 is not a quadratic residue
 *            modulo p. * * We generate primes by a sieve (build_primes_upto)
 *            and test each prime with * the Jacobi symbol. Primes for which
 *            (37|p) = -1 are stored in an * internally cached array. The
 *            cache is grown only when necessary. * * The fi...
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/build_primes_upto.h"

/* Internal cache of filtered primes. */
static seq_t  *filtered_primes = NULL;   /* array of primes p with (37|p) = -1 */
static size_t filtered_len   = 0;       /* number of stored primes  */
static idx_t  sieve_limit    = 0;       /* upper bound used for sieve   */

/* Build or extend the cache until it contains at least min_needed primes. */
static void
generate_filtered_primes(idx_t min_needed)
{
    /* If we already have enough, nothing to do */
    if (filtered_len >= (size_t)min_needed)
        return;

    /* Initial guess for sieve limit: 300 000 is more than enough for 10 000
       filtered primes (π(300 000) ≈ 25 000). */
    if (sieve_limit == 0)
        sieve_limit = 300000;

    while (1) {
        uint64_t *primes = NULL;
        size_t    prime_cnt = 0;
        /* Build all primes ≤ sieve_limit */
        build_primes_upto(sieve_limit, &primes, &prime_cnt);

        /* Allocate space for the filtered primes (worst case all primes). */
        free(filtered_primes);
        filtered_primes = (seq_t*)malloc(sizeof(seq_t) * prime_cnt);
        filtered_len    = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            bool ok;
            /* 2 is included by definition in the OEIS list. */
            if (p == 2)
                ok = true;
            else {
                int j = jacobi_symbol(37, p);
                ok = (j == -1);          /* 37 is a non‑residue modulo p */
            }
            if (ok)
                filtered_primes[filtered_len++] = (seq_t)p;
        }

        free(primes);

        /* Stop when we have enough filtered primes. */
        if (filtered_len >= (size_t)min_needed)
            return;

        /* Not enough – enlarge the sieve and try again. */
        sieve_limit *= 2;
    }
}

/* --------------------------------------------------------------------- */
/*  Main required function: seq_t a(idx_t n)  */
seq_t
a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for n=0 – return 0 as safe value */
        return (seq_t)0;

    /* Ensure that the cache contains at least n primes. */
    generate_filtered_primes(n);

    /* Return the requested term (array is 0‑based). */
    return filtered_primes[n - 1];
}
