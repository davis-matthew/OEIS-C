/*
 * A237669  Number of prime parts in the partitions of 3n into 3 parts.
 * https://oeis.org/A237669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime parts in the partitions of 3*n into 3
 *            parts
 * Range:     u8 n <= 18, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"


static uint64_t *primes = NULL;        /* array of primes */
static size_t   prime_cnt  = 0;        /* number of primes stored */
static idx_t    prime_lim  = 0;        /* highest number for which primes are generated */

/* Ensure that all primes up to 'limit' are available. */
static void ensure_primes(idx_t limit)
{
    if (prime_lim >= limit)
        return;

    free(primes);
    primes = NULL;
    prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_lim = limit;
}

seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 the sequence is undefined. */
    if (n == 0)
        return 0;

    const idx_t N = 3 * n;          /* sum of the parts */
    ensure_primes(N);

    seq_t total = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > N)
            break;

        const idx_t S = N - p;       /* sum of the other two parts */

        /* Contributions from partitions where p appears exactly once. */
        seq_t once = 0;
        if (S >= 2) {
            seq_t pairs = (seq_t)(S / 2);   /* unordered pairs (x,y), x>=y, x+y=S */
            if (S > p)                     /* the unique pair that contains p must be excluded */
                pairs -= 1;
            once = pairs;
        }

        /* Contributions from partitions where p appears exactly twice. */
        seq_t twice = 0;
        if (p != n && (N >= 2 * p + 1))   /* N-2p >= 1  and p is not the triple part */
            twice = 1;

        /* Contributions from the partition where p appears three times. */
        seq_t thrice = (p == n) ? 1 : 0;

        total += once + 2 * twice + 3 * thrice;
    }

    return total;
}
