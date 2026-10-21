/*
 * A076252  Integers k such that omega(k) = omega(k-1) + omega(k-2) +
 *          omega(k-3), where omega(n) is the number of distinct prime factors
 *          of n.
 * https://oeis.org/A076252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076252: Integers k such that ω(k) = ω(k-1) + ω(k-2) + ω(k-3)
 * (ω(k) := number of distinct prime factors of k).  This implementation
 * scans successive integers, computing ω(k) on the fly with the helper
 * count_distinct_primes(uint64_t) from "../utils/count_distinct_primes.h".
 * The sequence values are memoised in a static array to support
 * arbitrarily ordered calls to a(idx_t). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

static seq_t *series      = NULL;   /* memoised sequence values      */
static size_t series_len  = 0;       /* number of terms stored        */
static size_t series_cap  = 0;       /* allocated capacity of series  */

static uint64_t current_k       = 3; /* last integer examined          */
static int    omega_k_minus3 = 0;    /* ω(k-3) for last processed k   */
static int    omega_k_minus2 = 0;    /* ω(k-2)                         */
static int    omega_k_minus1 = 0;    /* ω(k-1)                         */

/* enlarge the series array as needed */
static void ensure_series_cap(size_t needed)
{
    if (series_cap >= needed) return;
    size_t new_cap = series_cap ? series_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(series, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    series = tmp;
    series_cap = new_cap;
}

/* initialise the first three ω values (k = 1,2,3) */
static void initialise(void)
{
    if (series_len != 0) return; /* already initialised */

    omega_k_minus3 = 0;                                    // ω(1)
    omega_k_minus2 = (int)count_distinct_primes(2ULL);      // ω(2)
    omega_k_minus1 = (int)count_distinct_primes(3ULL);      // ω(3)
    current_k = 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0 */

    initialise();

    /* generate terms until we have at least n of them */
    while (series_len < (size_t)n) {
        ++current_k;                                   /* next integer to test */
        int currOmega = (int)count_distinct_primes(current_k);

        if (currOmega == omega_k_minus1 + omega_k_minus2 + omega_k_minus3) {
            ensure_series_cap(series_len + 1);
            series[series_len++] = (seq_t)current_k;
        }

        /* shift ω values for the next step */
        omega_k_minus3 = omega_k_minus2;
        omega_k_minus2 = omega_k_minus1;
        omega_k_minus1 = currOmega;
    }

    return series[n - 1];
}
