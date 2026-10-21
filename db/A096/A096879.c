/*
 * A096879  a(1) = 1; for n > 1: a(n) = least multiple m of n such that m is
 *          coprime to n+1 and the absolute difference of a(n) and a(n-1) is a
 *          prime distinct from all earlier such differences of consecutive
 *          terms.
 * https://oeis.org/A096879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursively: a(1) = 1; For n > 1, a(n) is the
 *            least multiple of n that is coprime to n+1 and whose absolute
 *            difference from a(n‑1) is a prime distinct from all previous
 *            such differences. The function below computes this sequence,
 *            memoising previously computed terms and primes
 * Range:     u8 n <= 16, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"           /* gcd(seq_t, seq_t) */
#include "../utils/isPrimeGeneric.h" /* isPrimeGeneric(seq_t) */

seq_t a(idx_t n) {
    /* Static cache for computed a(n) values (1‑based). */
    static seq_t *a_cache = NULL;   /* pointer to array of seq_t */
    static size_t a_cap = 0;        /* allocated capacity */
    static size_t a_len = 0;        /* number of terms computed (a_cache[1..a_len]) */

    /* Static cache for distinct prime differences used so far. */
    static seq_t *diff_cache = NULL;
    static size_t diff_cap = 0;
    static size_t diff_len = 0;     /* number of distinct primes stored */

    /* Helper: grow a cache to at least ‘need’ capacity. */
    if (n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = a_len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1) {                  /* base case */
            val = (seq_t)1;
            /* No difference to record for the first term. */
        } else {
            seq_t prev = a_cache[i - 1];
            seq_t m = 0;
            idx_t k = 1;
            while (true) {
                m = (seq_t)k * (seq_t)i;                     /* kth multiple of n */
                if (gcd(m, (seq_t)(i + 1)) != 1) {            /* must be coprime to n+1 */
                    ++k;
                    continue;
                }
                seq_t diff = (m >= prev) ? m - prev : prev - m;
                if (!isPrimeGeneric(diff)) {                  /* difference must be prime */
                    ++k;
                    continue;
                }
                /* Check that this prime difference has not appeared before. */
                bool used = false;
                for (size_t j = 0; j < diff_len; ++j) {
                    if (diff_cache[j] == diff) { used = true; break; }
                }
                if (used) {
                    ++k;
                    continue;
                }
                /* Found valid m */
                val = m;
                /* Store the new prime difference. */
                if (diff_len == diff_cap) {
                    size_t d_new_cap = diff_cap ? diff_cap * 2 : 8;
                    diff_cache = (seq_t *)realloc(diff_cache,
                                                   d_new_cap * sizeof(seq_t));
                    diff_cap = d_new_cap;
                }
                diff_cache[diff_len++] = diff;
                break;
            }
        }
        a_cache[i] = val;
    }
    a_len = (size_t)n;          /* update number of computed terms */
    return a_cache[(size_t)n];
}
