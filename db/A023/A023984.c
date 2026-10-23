/*
 * A023984  Sum of exponents in prime-power factorization of multinomial
 *          coefficient M(6n; n,n,n,n,n,n).
 * https://oeis.org/A023984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_p [ ⌊(6n)/p^k⌋ − 6·⌊n/p^k⌋ ] over all primes p and k
 *            ≥ 1. * Equivalently, a(n) = Ω((6n)! ) − 6·Ω(n!) where Ω(m) is
 *            the total * number of prime factors of m (counted with
 *            multiplicity). This is * the sum of exponents in the prime‑power
 *            factorization of the * multinomial coefficient M(6n...
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

/* Cached list of primes up to the current maximum limit reached. */
static idx_t *prime_cache = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure primes up to ‘limit’ are available. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                      /* already have enough primes */

    /* Rebuild prime list up to the new limit. */
    if (prime_cache != NULL)
        free(prime_cache);            /* free old list */

    build_primes(limit, &prime_cache, &prime_count);
    prime_limit = limit;
}

seq_t
a(idx_t n)
{
    /* Convert n to seq_t for calculations involving seq_t. */
    seq_t n_seq = (seq_t)n;
    seq_t six_n = n_seq * (seq_t)6;

    /* Make sure we have all primes ≤ 6n. */
    ensure_primes((idx_t)six_n);

    seq_t total = 0;

    for (size_t i = 0; i < prime_count; ++i)
    {
        /* primes are stored as idx_t; cast to seq_t for arithmetic. */
        seq_t p = (seq_t)prime_cache[i];
        /* Iterate over powers of p: p, p², p³, … while ≤ 6n. */
        seq_t power = p;
        while (power <= six_n)
        {
            seq_t a = six_n / power;
            seq_t b = n_seq / power;
            total += a - (seq_t)6 * b;
            /* Advance to next power: stop if multiplication would overflow */
            if (power > six_n / p)
                break;
            power *= p;
        }
    }

    return total;
}
