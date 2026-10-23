/*
 * A139102  Numbers whose binary representation shows the distribution of
 *          prime numbers up to the n-th prime minus 1, using "0" for primes
 *          and "1" for nonprime numbers.
 * https://oeis.org/A139102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the binary string that lists numbers 1 …
 *            pₙ−1, * with bit 0 for primes and bit 1 for non‑prime numbers. *
 *            The nth prime is pₙ; we use the helper _prime_limit_for_idx to
 *            decide * how far to sieve and build_primes_upto to obtain the
 *            list of primes
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"   /* idx_t _prime_limit_for_idx(idx_t n); */
#include "../utils/build_primes_upto.h"      /* void build_primes_upto(idx_t limit,
                                            uint64_t **primes_out, size_t *cnt_out); */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;      /* number of primes stored */
static idx_t  sieve_limit  = 0;      /* maximum index already sieved up to */

/* Ensure that prime_list contains at least 'n' primes. */
static void
ensure_sieve(idx_t n)
{
    if (n <= prime_count)            /* already have enough primes */
        return;

    /* Compute an upper bound on the n-th prime and sieve up to it. */
    idx_t limit = _prime_limit_for_idx(n);
    if (limit <= sieve_limit)        /* previously sieved far enough */
        return;

    uint64_t *new_list;
    size_t    new_count;
    build_primes_upto(limit, &new_list, &new_count);

    /* Replace old list (leaking is fine for a short test program). */
    free(prime_list);
    prime_list   = new_list;
    prime_count  = new_count;
    sieve_limit  = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1 – undefined for n=0 */

    ensure_sieve(n);

    seq_t res = 0;
    uint64_t nth_prime = prime_list[n-1];
    uint64_t upper     = nth_prime - 1;   /* inclusive upper bound for bits */

    size_t p_index = 0;   /* current index in prime_list */
    for (uint64_t i = 1; i <= upper; ++i) {
        bool isprime = false;
        if (p_index < prime_count && prime_list[p_index] == i) {
            isprime = true;
            ++p_index;
        }
        /* Bit 0 for primes, 1 for non‑prime (including 1). */
        res = (res << 1) | (isprime ? 0 : 1);
    }
    return res;
}
