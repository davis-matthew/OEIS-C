/*
 * A102863  a(n)=1 if at least one of the first n primes is a divisor of the
 *          sum of the first n primes; otherwise a(n)=0.
 * https://oeis.org/A102863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8964 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = 1 if some of the first n primes divides the sum of the first n primes,
 *        0 otherwise.  Compute by generating the first n primes with the
 *        sieve of Eratosthenes and testing divisibility.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* -------------------------------------------------------------------- */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;  /* covers first 6 primes                        */
    double nn    = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint64_t)(bound + 3.0);  /* safety margin                         */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* safety – OEIS offset is 1 */
        return (seq_t)0;

    /* estimate an upper bound that contains the nth prime */
    uint64_t limit = estimate_prime_upper_bound(n);
    if (limit < 2) limit = 2;

    /* Sieve array: use malloc and cast explicitly for C++ compatibility */
    bool *isComposite = (bool *)malloc((size_t)(limit + 1));
    if (!isComposite) return (seq_t)0;
    memset(isComposite, 0, (size_t)(limit + 1));

    /* storage for the first n primes and cumulative sums */
    uint64_t *primes   = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    seq_t    *prefix   = mem_new(seq_t, (size_t)n);
    if (!primes || !prefix) {
        free(isComposite);
        free(primes);
        mem_free(prefix);
        return (seq_t)0;
    }

    idx_t  count = 0;
    seq_t  sum   = 0;

    for (uint64_t i = 2; i <= limit && count < n; ++i) {
        if (!isComposite[i]) {
            /* record the prime */
            primes[count] = i;
            sum += (seq_t)i;
            prefix[count] = sum;
            ++count;

            /* mark multiples of i */
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    isComposite[j] = true;
            }
        }
    }

    if (count < n) {               /* insufficient primes – sanity check */
        free(isComposite);
        free(primes);
        mem_free(prefix);
        return (seq_t)0;
    }

    seq_t total = prefix[n - 1];
    bool  found = false;

    for (idx_t i = 0; i < n; ++i) {
        if (total % (seq_t)primes[i] == (seq_t)0) {
            found = true;
            break;
        }
    }

    /* clean up */
    free(isComposite);
    free(primes);
    mem_free(prefix);

    return found ? (seq_t)1 : (seq_t)0;
}
