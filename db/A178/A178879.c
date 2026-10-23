/*
 * A178879  a(n) = number of 8-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..71].
 * https://oeis.org/A178879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts of 8‑digit primes by digit sum.
   For n = 1,2,... the sum is the (n‑th) integer in [2..71] that is
   not a multiple of 3.  a(n) is the number of 8‑digit primes whose
   digits sum to that value.  The function is computed by sieving all
   primes up to 99 999 999, then counting digit sums of the 8‑digit
   primes found. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility that builds all primes <= limit and returns an array of uint64_t. */
#include "../utils/build_primes_upto.h"

/* static cache of counts indexed by digit sum (0..71) */
static seq_t *digit_sum_counts = NULL;
/* array giving the non‑multiples of 3 in [2..71] in order */
static unsigned short sums_values[72];
static size_t sums_count = 0;
/* one‑time initialization flag */
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Helper: initialise the caches on first call of a()                  */
/* ------------------------------------------------------------------ */
static void init_counts(void)
{
    /* list the sums that are not multiples of 3 */
    sums_count = 0;
    for (unsigned int s = 2; s <= 71; ++s)
        if (s % 3 != 0)
            sums_values[sums_count++] = (unsigned short)s;

    /* memory for 72 possible sums, zeroed */
    digit_sum_counts = (seq_t *)calloc(72, sizeof(seq_t));

    /* sieve all primes up to 99,999,999 */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(99999999ULL, &primes, &prime_cnt);

    /* count digit sums of the 8‑digit primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p < 10000000ULL)      /* fewer than 8 digits */
            continue;
        /* compute digit sum */
        uint64_t tmp = p;
        unsigned int sum = 0;
        while (tmp) {
            sum += (unsigned int)(tmp % 10ULL);
            tmp /= 10ULL;
        }
        if (sum <= 71)
            digit_sum_counts[sum]++;   /* wrap on overflow, as allowed */
    }

    /* clean up */
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Sequence function as required                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_counts();
        initialized = true;
    }

    if (n == 0)          /* unused index – keep type‑analyzer happy */
        return (seq_t)0;

    /* convert OEIS index n (starting at 1) to the sum value */
    idx_t idx = n - 1;               /* zero‑based in sums_values */
    if (idx >= sums_count)          /* beyond 71 or non‑mult‑of‑3 */
        return (seq_t)0;

    unsigned short sum = sums_values[(size_t)idx];
    return digit_sum_counts[(size_t)sum];
}
