/*
 * A215995  Primes that are the sum of 201 consecutive primes.
 * https://oeis.org/A215995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime that can be expressed as the sum of 201 consecutive primes. */
/* The function pre‑computes all such primes up to a pre‑defined limit (6 M) using a sliding window and tests each sum for primality. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/append_candidate.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static tables for primes, results and bookkeeping */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    if (!initialized) {
        /* 6 million is more than enough to generate > 15 000 terms. */
        const size_t LIMIT = 6000000;
        const size_t WINDOW = 201;

        /* Build list of primes up to LIMIT */
        build_primes_upto(LIMIT, &primes, &primes_cnt);

        /* Sliding‑window sum of length WINDOW */
        seq_t sum = 0;
        for (size_t i = 0; i < WINDOW; ++i) {
            sum += (seq_t)primes[i];
        }

        /* First window */
        if (isPrimeGeneric(sum)) {
            append_candidate(&terms, &terms_len, &terms_cap, sum);
        }

        /* Subsequent windows */
        for (size_t i = WINDOW; i < primes_cnt; ++i) {
            sum += (seq_t)primes[i];
            sum -= (seq_t)primes[i - WINDOW];
            if (isPrimeGeneric(sum)) {
                append_candidate(&terms, &terms_len, &terms_cap, sum);
            }
        }

        initialized = true;
    }

    if (n == 0 || n > (idx_t)terms_len) {
        /* Out of range – May return 0 or handle as needed. */
        return (seq_t)0;
    }
    return terms[(size_t)(n - 1)];
}
