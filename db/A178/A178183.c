/*
 * A178183  a(n) = number of 3-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..26].
 * https://oeis.org/A178183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   build_primes_upto, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts 3‑digit primes for each digit sum that is a non‑multiple of
   3 in the range 2 … 26.  a(1) corresponds to digit sum 2, a(2) to 4,
   etc.  The table of sums is fixed; the prime counts are computed lazily
   on the first call. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"

/* The 17 non‑multiples of 3 between 2 and 26 (inclusive).  This is the
   OEIS mapping from index to digit sum. */
static const uint8_t nonm3[17] = {
    2,4,5,7,8,10,11,13,14,16,17,19,20,22,23,25,26
};

/* counts[s] = number of 3‑digit primes with digit sum s (s∈[2..26]) */
static seq_t counts[27] = {0};
static bool initialized = false;

/* Initialise the counts array.  This is done only once. */
static void init_counts(void)
{
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;

    /* Generate all primes up to 999 (inclusive).  This includes all
       3‑digit primes 100…999. */
    build_primes_upto(999, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p < 100) continue;                   /* skip <100 */
        if (p > 999) break;                      /* should never happen */

        seq_t s = digit_sum((seq_t)p);           /* digit sum, 2…26 */
        if (s <= 26)                             /* safety */
            counts[(size_t)s] = counts[(size_t)s] + 1;
    }

    free(primes);
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_counts();

    if (n < 1 || n > 17)
        return 0;      /* out of OEIS range */

    uint8_t sum = nonm3[(size_t)(n - 1)];
    return counts[(size_t)sum];
}
