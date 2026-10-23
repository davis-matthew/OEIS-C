/*
 * A101549  Composite lopsided numbers: composite numbers n such that the
 *          largest prime factor > 2 sqrt(n).
 * https://oeis.org/A101549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101549
   Composite lopsided numbers: composite n such that the largest prime factor > 2*sqrt(n).
   This implementation searches sequentially for such numbers.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"   /* path is kept to match the build environment */

/* Upper bound for the sieve. 2 000 000 gives >140 000 primes and covers the
   required sqrt range for the test data. */
#define SIEVE_MAX 2000000

static unsigned char sieve[SIEVE_MAX + 1];
static idx_t primes_arr[SIEVE_MAX / 10 + 1];   /* enough to hold all primes <= SIEVE_MAX */
static size_t primes_cnt = 0;
static bool sieve_done = false;

/* Initialise the prime list once. */
static void init_primes(void)
{
    if (sieve_done) return;
    for (idx_t i = 0; i <= SIEVE_MAX; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t i = 2; i * i <= SIEVE_MAX; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_MAX; j += i) sieve[j] = 0;
        }
    }

    for (idx_t i = 2; i <= SIEVE_MAX; ++i) {
        if (sieve[i]) primes_arr[primes_cnt++] = i;
    }
    sieve_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_primes();

    idx_t count = 0;
    idx_t candidate = 4;                    /* first composite number */

    while (count < n) {
        bool is_prime = true;
        seq_t rem = (seq_t)candidate;
        seq_t lpf = 0;                      /* largest prime factor */

        for (size_t i = 0;
             i < primes_cnt && (idx_t)primes_arr[i] * (idx_t)primes_arr[i] <= candidate;
             ++i) {
            idx_t p = primes_arr[i];
            if (rem % (seq_t)p == 0) {
                is_prime = false;
                while (rem % (seq_t)p == 0) {
                    lpf = (seq_t)p;
                    rem /= (seq_t)p;
                }
            }
        }

        if (is_prime) {                    /* skip primes */
            candidate++;
            continue;
        }

        if (rem > 1) lpf = rem;             /* remainder is a prime factor */

        /* Condition: largest prime factor > 2*sqrt(n)  <=>  lpf^2 > 4*n */
        seq_t lhs = lpf * lpf;
        seq_t rhs = (seq_t)4 * (seq_t)candidate;
        if (lhs > rhs) {
            count++;
            if (count == n) return (seq_t)candidate;
        }

        candidate++;
    }
    return 0;                              /* unreachable for valid n */
}
