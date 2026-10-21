/*
 * A216308  Values of k such that 10*k+7 and 10*k+9 are the only primes
 *          between 10*k and 10*k+9.
 * https://oeis.org/A216308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A216308.
   a(n) is the n-th integer k such that the only primes in [10*k,10*k+9]
   are 10*k+7 and 10*k+9.  The function searches k sequentially, using
   a fast sieve to test primality.  (No closed‑form formula is known.)   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* global sieve */
static idx_t prime_limit = 0;   /* maximum index that prime_table covers   */
static bool *prime_table = NULL;

/* Ensure that prime_table contains primality information up to at least
   `req`.  The table is resized and reconstructed in powers of two. */
static void
ensure_prime_limit(idx_t req)
{
    if (req <= prime_limit && prime_table != NULL)
        return;

    /* choose a new limit that is a power of two and >= req */
    idx_t new_lim = prime_limit ? prime_limit * 2 : 100000u;
    while (new_lim < req)
        new_lim *= 2;

    bool *new_tbl = (bool *)malloc(new_lim + 1);
    if (!new_tbl) abort();

    /* initialise all as prime */
    for (idx_t i = 0; i <= new_lim; ++i)
        new_tbl[i] = true;
    if (new_lim >= 0) new_tbl[0] = false;
    if (new_lim >= 1) new_tbl[1] = false;

    /* sieve */
    for (idx_t p = 2; (p * p) <= new_lim; ++p) {
        if (!new_tbl[p]) continue;
        for (idx_t m = p * p; m <= new_lim; m += p)
            new_tbl[m] = false;
    }

    free(prime_table);
    prime_table = new_tbl;
    prime_limit = new_lim;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – a(0) not used */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t start = 10 * k;
        idx_t n7   = start + 7;
        idx_t n9   = start + 9;

        /* make sure the sieve covers everything we need to test */
        ensure_prime_limit(n9);

        if (!prime_table[n7] || !prime_table[n9])
            continue;
        if (prime_table[start + 1] || prime_table[start + 3])
            continue;

        ++count;
        if (count == n)
            return (seq_t)k;
    }
}
