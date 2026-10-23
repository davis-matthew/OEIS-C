/*
 * A173112  Partial sums of A106116.
 * https://oeis.org/A173112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} p_i where p_i is the i‑th prime that
 *            contains no digit 0 or 1 (decimal). a(1)=2, a(2)=5, …
 * Range:     u8 n <= 10, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_no_digit_zero_or_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_no_digit_zero_or_one.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Storage for the list of good primes and their prefix sums.                 */
static seq_t *good_primes   = NULL;   /* primes with no 0 or 1 */
static size_t good_cnt      = 0;      /* number of such primes stored */
static size_t good_cap      = 0;      /* allocated capacity of good_primes */

static seq_t *prefix_sums   = NULL;   /* prefix_sums[i] = sum of first i good primes, prefix_sums[0] = 0 */
static size_t prefix_size   = 0;      /* allocated size of prefix_sums (good_cnt + 1) */

/* -------------------------------------------------------------------------- */
/* Build tables on first call to a(n).  LIMIT is chosen large enough to
   contain at least 10 000 good primes (which is sufficient for n ≤ 10 000).  */
static void build_tables(void)
{
    /* Limit such that we obtain at least 10 000 primes without digits 0 or 1. */
    const idx_t LIMIT = 2000000U;   /* 2 million → memory ~2 MB, enough for ~10 k good primes */

    /* Sieve of Eratosthenes. */
    bool *is_composite = (bool*)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_composite) {
        /* Allocation failed – leave tables empty. */
        return;
    }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            /* i is prime */
            if (has_no_digit_zero_or_one(i)) {
                if (good_cnt >= good_cap) {
                    good_cap = good_cap ? good_cap * 2 : 1024;
                    good_primes = (seq_t*)realloc(good_primes, good_cap * sizeof(seq_t));
                    if (!good_primes) {
                        free(is_composite);
                        return;
                    }
                }
                good_primes[good_cnt++] = (seq_t)i;
            }
            if (i <= LIMIT / i) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);

    /* Build prefix sums. */
    prefix_size = good_cnt + 1;
    prefix_sums = (seq_t*)calloc(prefix_size, sizeof(seq_t));
    if (!prefix_sums)
        return;

    seq_t acc = (seq_t)0;
    prefix_sums[0] = acc;   /* sum of 0 terms */
    for (size_t i = 0; i < good_cnt; ++i) {
        acc += good_primes[i];
        prefix_sums[i + 1] = acc;
    }
}

/* -------------------------------------------------------------------------- */
/* Public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Respect OEIS offset: a(0) is unused; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Lazily build tables on first use. */
    if (!prefix_sums)
        build_tables();

    if (!prefix_sums || (size_t)n >= prefix_size)
        return (seq_t)0;   /* insufficient data (should not happen for n ≤ 10 000) */

    return prefix_sums[(size_t)n];
}
