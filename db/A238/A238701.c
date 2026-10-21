/*
 * A238701  Number of primes p < n with q = floor((n-p)/4) and q^2 - 2 both
 *          prime.
 * https://oeis.org/A238701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1937, u32 n <= 1291, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238701:
   a(n) = #{p < n | p prime ∧ q = floor((n − p)/4) is prime ∧ q²−2 is prime}
   (offset 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static uint64_t *prime_arr = NULL; /* array of primes < prime_lim */
static size_t   prime_cnt  = 0;
static idx_t    prime_lim   = 0;

/* Ensure we have all primes < n in prime_arr.  Grow the table if needed. */
static void
ensure_primes(idx_t n)
{
    if (prime_lim >= n)
        return;

    /* Allocate a slightly larger table to avoid frequent reallocations. */
    idx_t new_lim = n + 1000;
    uint64_t *p    = NULL;
    size_t     cnt = 0;

    build_primes_upto(new_lim, &p, &cnt);
    free(prime_arr);
    prime_arr = p;
    prime_cnt = cnt;
    prime_lim = new_lim;
}

seq_t a(idx_t n)
{
    /* For the defined offset, a(1) = 0; for n < 1 the function is unused. */
    if (n <= 1)
        return (seq_t)0;

    ensure_primes(n);

    seq_t count = 0;

    /* Iterate over all primes p < n. */
    for (size_t i = 0; i < prime_cnt && prime_arr[i] < n; ++i) {
        idx_t p = prime_arr[i];
        idx_t q = (n - p) / 4;          /* floor((n−p)/4) */
        if (q < 2)                       /* q must be at least 2 to be prime */
            continue;

        seq_t qseq = (seq_t)q;
        if (!is_prime_big(qseq))          /* q must be prime */
            continue;

        seq_t qsq   = qseq * qseq;
        seq_t test  = qsq - (seq_t)2;
        if (is_prime_big(test))
            ++count;
    }
    return count;
}
