/*
 * A181834  The number of primes <= n that are strongly prime to n.
 * https://oeis.org/A181834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {p prime ≤ n | gcd(p,n)=1 ∧ (n-1) mod p ≠ 0}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* array of primes, sorted ascending */
static size_t primes_cnt = 0;     /* number of primes in the array */
static idx_t primes_limit = 0;    /* largest n for which primes are computed */

/* Extend the primes array so that it contains all primes ≤ limit. */
static void extend_primes(idx_t limit)
{
    if (limit <= primes_limit) return;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    free(primes); /* free old array if any */
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* Count primes <= n that are strongly prime to n. */
seq_t a(idx_t n)
{
    /* Handle trivial small n */
    if (n < 2) return (seq_t)0;

    /* Ensure prime list is available up to n */
    extend_primes(n);

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;

        seq_t p_seq = (seq_t)p;
        if (gcd(n_seq, p_seq) != 1) continue;
        if ((n - 1) % p == 0) continue;

        ++count;
    }

    return count;
}
