/*
 * A071966  Composite numbers such that smallest prime factor, largest prime
 *          factor and sum of prime factors (with repetition) are all a sum of
 *          two squares.
 * https://oeis.org/A071966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite number whose smallest prime factor,
 *            largest prime factor and sum of prime factors (with repetition)
 *            are all sums of two squares
 * Range:     u8 n <= 23, u16 n <= 5979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_of_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sum_of_two_squares.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static uint64_t *spf = NULL;      /* 0‑based index, size = LIMIT+1 */
    static seq_t *sopfr = NULL;       /* sum of prime factors with repetition */
    const size_t LIMIT = 500000;      /* enough for n up to 10 000 */

    /* Build sieve, spf and sopfr once */
    if (!initialized) {
        spf = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
        if (!spf) return 0;          /* allocation failure (rare) */
        spf[1] = 1;
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {       /* i is prime */
                spf[i] = i;
                if (i <= LIMIT / i)  /* avoid overflow in j = i*i */
                    for (size_t j = i * i; j <= LIMIT; j += i)
                        if (!spf[j])
                            spf[j] = i;
            }
        }
        sopfr = (seq_t *)calloc(LIMIT + 1, sizeof(seq_t));
        if (!sopfr) return 0;
        sopfr[0] = sopfr[1] = 0;
        for (size_t i = 2; i <= LIMIT; ++i) {
            uint64_t p = spf[i];
            sopfr[i] = sopfr[i / p] + (seq_t)p;
        }
        initialized = true;
    }

    /* Validate arguments */
    if (n == 0) return 0;

    size_t count = 0;
    for (size_t m = 4; m <= LIMIT; ++m) {
        /* Skip primes */
        if (spf[m] == m)
            continue;

        /* Smallest prime factor */
        uint64_t spf_m = spf[m];
        if (!(spf_m == 2 || (spf_m & 3) == 1))
            continue;

        /* Largest prime factor */
        uint64_t temp = m;
        uint64_t lpf = 0;
        while (temp > 1) {
            uint64_t p = spf[temp];
            lpf = p;
            temp /= p;
        }
        if (!(lpf == 2 || (lpf & 3) == 1))
            continue;

        /* Sum of prime factors (with repetition) */
        seq_t sop = sopfr[m];
        if (!sum_of_two_squares((uint64_t)sop, spf))
            continue;

        /* All conditions satisfied */
        ++count;
        if (count == (size_t)n)
            return (seq_t)m;
    }

    /* If we reach here, the chosen LIMIT was insufficient.  */
    return 0;
}
