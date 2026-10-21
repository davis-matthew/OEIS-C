/*
 * A087916  Number of ordered ways to write 2n+1 as a sum of 3 odd primes.
 * https://oeis.org/A087916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 2152, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Count ordered triples of odd primes (p1,p2,p3) with
 *           p1 + p2 + p3 = 2*n + 1
 * Implementation: iterate over all ordered pairs of odd primes
 *                 and check the third prime by lookup in a boolean
 *                 sieve.  The sieve and prime list are lazily
 *                 built up to the needed maximum target.  This
 *                 works in O(π(T)^2) time per query with T = 2*n+1
 *                 and needs O(T) space, which is well below
 *                 the 2 s limit for n ≤ 10000.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state for the prime sieve */
static size_t prime_limit = 0;
static uint8_t *prime_bool = NULL;   /* 1 = prime, 0 = composite */
static uint32_t *primes = NULL;      /* list of odd primes (≥3) */
static size_t primes_count = 0;

/* ensure the sieve contains all primes up to 'limit' inclusive */
static void ensure_primes(idx_t limit) {
    if ((idx_t)prime_limit >= limit)
        return;

    /* reallocate sieve array */
    size_t new_limit = (size_t)limit;
    uint8_t *new_bool = (uint8_t *)malloc(new_limit + 1);
    if (!new_bool) {
        /* allocation failure – abort */
        abort();
    }
    /* initialize all as prime first */
    for (size_t i = 0; i <= new_limit; ++i)
        new_bool[i] = 1;
    if (new_limit >= 0) new_bool[0] = 0;
    if (new_limit >= 1) new_bool[1] = 0;

    /* sieve of Eratosthenes */
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_bool[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_bool[j] = 0;
        }
    }

    /* build prime list (odd primes only) */
    uint32_t *new_primes = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    if (!new_primes) {
        free(new_bool);
        abort();
    }
    size_t cnt = 0;
    for (size_t i = 3; i <= new_limit; i += 2) {
        if (new_bool[i]) {
            new_primes[cnt++] = (uint32_t)i;
        }
    }

    /* replace old data */
    free(prime_bool);
    free(primes);
    prime_bool = new_bool;
    primes = new_primes;
    primes_count = cnt;
    prime_limit = new_limit;
}

seq_t a(idx_t n) {
    /* smallest 2*n+1 that can be represented is 9 (n=4) */
    if (n < 4)
        return 0;

    idx_t target = 2 * n + 1;
    ensure_primes(target);

    seq_t count = 0;

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p1 = primes[i];
        if ((idx_t)p1 > target)
            break;
        for (size_t j = 0; j < primes_count; ++j) {
            uint32_t p2 = primes[j];
            idx_t sum12 = (idx_t)p1 + (idx_t)p2;
            if (sum12 > target - 3)  /* because p3 must be at least 3 */
                break;
            idx_t p3 = target - sum12;
            if (p3 >= 3 && p3 <= (idx_t)prime_limit && prime_bool[(size_t)p3]) {
                count += 1;
            }
        }
    }
    return count;
}
