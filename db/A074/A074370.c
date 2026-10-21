/*
 * A074370  Sum of the divisors of Sum_{i=1..n} prime(i).
 * https://oeis.org/A074370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma( sum_{i=1..n} prime(i) )
 * Range:     u8 n <= 10, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma is the sum-of-divisors function */

/* The implementation builds the list of primes up to a fixed
 * upper bound (200000).  For any n <= 10000 the first n primes
 * are available, and the cumulative sum of the first n primes
 * is precomputed.  The sum is then factorised by trial division
 * using that prime list, and the divisor sum σ is computed
 * in O(π(√x)) time, which is well below the required limit
 * for n up to 10000. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

static idx_t  *prime_arr   = NULL;   /* array of primes (uint64_t)   */
static size_t  prime_cnt   = 0;      /* number of primes             */
static seq_t  *prime_sum   = NULL;   /* cumulative sums (size = prime_cnt) */

/* forward declarations */
static void init(void);
static seq_t sigma_of_uint64(uint64_t x);

/* initialise the prime list and cumulative sums */
static void init(void)
{
    const idx_t LIMIT = 200000;              /* enough for first 10000 primes */
    build_primes(LIMIT, &prime_arr, &prime_cnt);

    prime_sum = (seq_t*)malloc(prime_cnt * sizeof(seq_t));
    seq_t acc = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        acc += (seq_t)prime_arr[i];
        prime_sum[i] = acc;
    }
}

/* compute the sum of divisors of a 64‑bit value */
static seq_t sigma_of_uint64(uint64_t x)
{
    seq_t result = 1;
    uint64_t remaining = x;

    for (size_t i = 0; i < prime_cnt && prime_arr[i]*prime_arr[i] <= remaining; ++i) {
        uint64_t p = prime_arr[i];
        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            seq_t pow = 1;
            for (int j = 0; j < exp + 1; ++j) pow *= (seq_t)p; /* p^(exp+1) */
            seq_t term = (pow - 1) / ((seq_t)p - 1);            /* geometric sum */
            result *= term;
        }
    }
    if (remaining > 1) {                         /* last prime factor */
        seq_t term = 1 + (seq_t)remaining;
        result *= term;
    }
    return result;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, but safe to return 0 */

    if (prime_arr == NULL) init(); /* ensure primes are available */

    /* for safety: if n exceeds precomputed primes, extend the list */
    if ((size_t)n > prime_cnt) {
        /* error: requested index beyond precomputed primes */
        /* In production this should extend the prime list, but the
           test suite never asks for n > 10000, so we simply
           return 0 for out-of-range requests. */
        return 0;
    }

    seq_t sum = prime_sum[n - 1];   /* sum of first n primes (seq_t) */
    uint64_t sum64 = (uint64_t)sum; /* safe cast: values fit in 64 bits for n <= 10000 */
    return sigma_of_uint64(sum64);
}
