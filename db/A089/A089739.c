/*
 * A089739  Let b(m)= base ten expansion of prime(n); then
 *          a(n)=Sum[Mod(10-b(l),10)*10^l,{l,1,10}].
 * https://oeis.org/A089739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = Sum_{l=1}^{10} (10 - d_l) mod 10 * 10^{l-1}, where the
 *            digits d_l are the decimal digits of the prime with index (n-1).
 *            Thus a(2) is the complement of prime(1)=2, a(6) is the
 *            complement of prime(5)=11, etc
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_primes.h"

/* static state to hold the list of primes */
static idx_t *prime_list = NULL;   /* array of primes in ascending order */
static size_t prime_count = 0;     /* number of primes stored */
static int primes_initialized = 0;

/* initialise the prime list up to a generous upper bound.
   The 10 000th prime is 104729 < 200 000, so this suffices for all calls. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* 200 000 is more than enough for the 10 000th prime. */
    idx_t limit = 200000;

    /* sieve_primes will allocate memory for primes and set the count */
    sieve_primes(limit, &prime_list, &prime_count);
    /* sanity check: ensure we have at least the first 10 000 primes */
    if (prime_count < 10000) {
        /* in the unlikely event the bound was too small, grow the sieve */
        idx_t new_limit = limit * 2;
        while (prime_count < 10000) {
            /* free previous list and sieve again */
            free(prime_list);
            sieve_primes(new_limit, &prime_list, &prime_count);
            new_limit *= 2;
        }
    }
    primes_initialized = 1;
}

/* compute a(n) as defined in OEIS A089739 */
seq_t a(idx_t n)
{
    /* A089739 is 1-indexed; the first term is 9 (complement of 1). */
    if (n == 1)
        return (seq_t)9;

    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1; return 0 */

    init_primes();               /* ensure prime list is ready */
    idx_t m = n - 2;             /* index into prime_list: prime(n-1) = prime_list[n-2] */

    if (m >= prime_count) {
        /* safety: generate more primes if necessary (unlikely for n <= 10 000) */
        idx_t limit = prime_list[prime_count - 1] + 1000;
        free(prime_list);
        sieve_primes(limit, &prime_list, &prime_count);
        if (m >= prime_count)
            return (seq_t)0;   /* still not enough */
    }

    uint64_t p = prime_list[m];
    seq_t result = 0;
    uint64_t factor = 1;

    while (p > 0) {
        uint64_t digit = p % 10;
        uint64_t comp = (10 - digit) % 10;   /* complement of the digit */
        result += comp * factor;
        factor *= 10;
        p /= 10;
    }
    return result;
}
