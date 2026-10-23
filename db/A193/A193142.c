/*
 * A193142  Primes which are the sum of 5 distinct positive squares.
 * https://oeis.org/A193142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193142: primes that are the sum of five distinct positive squares.
   Formula: a(n) is computed by enlarging a sieve bound and marking all
   sums of 5 distinct squares with dynamic programming. */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* declares build_primes */

/* Find the nth term of A193142.  The function caches all found terms,
   so subsequent calls are O(1).  It works for any seq_t and idx_t
   supplied by the build. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms          = NULL;   /* sequence values */
    static size_t terms_count    = 0;      /* number of terms cached   */
    static size_t terms_capacity = 0;      /* allocated capacity       */
    static size_t last_limit     = 0;      /* upper bound of last sieve */

    /* Convert the index to a size_t once, avoiding unsigned underflow. */
    size_t requested = (size_t)n;
    if (requested == 0) return 0;         /* undefined for n < 1 */

    /* Ensure we have enough terms. */
    while (terms_count < requested) {

        /* Grow the sieve bound.  200 is a modest initial value. */
        size_t new_limit = (last_limit < 200) ? 200 : last_limit * 2;

        /* ----------------------------------------------------------------
           1.  Compute all sums of 5 distinct squares up to new_limit
           ----------------------------------------------------------------*/
        /* Determine the largest square root. */
        int max_i = 0;
        while ((max_i + 1) * (max_i + 1) <= (int)new_limit) ++max_i;

        /* Allocate DP arrays: dp[k][s] == true  ⇔  s can be expressed as the sum
           of k distinct squares.  We only need k = 0..5. */
        char *dp[6];
        for (int k = 0; k <= 5; ++k) {
            dp[k] = (char *)calloc((size_t)new_limit + 1, sizeof(char));
            if (!dp[k]) return 0;      /* allocation failure → sentinel */
        }
        dp[0][0] = 1; /* zero squares sum to 0 */

        for (int i = 1; i <= max_i; ++i) {
            int sq = i * i;
            if (sq > (int)new_limit) break;

            /* Update DP tables from high k down to 1 to avoid reuse of the same square. */
            for (int k = 5; k >= 1; --k) {
                int lim = (int)new_limit - sq;
                for (int s = 0; s <= lim; ++s) {
                    if (dp[k - 1][s]) {
                        dp[k][s + sq] = 1;
                    }
                }
            }
        }

        /* ----------------------------------------------------------------
           2.  Generate primes up to new_limit and collect qualifying ones
           ----------------------------------------------------------------*/
        idx_t *prime_list = NULL;
        size_t prime_cnt = 0;
        build_primes((idx_t)new_limit, &prime_list, &prime_cnt);

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > new_limit) break;
            if (p <= last_limit) continue;          /* already processed */
            if (dp[5][(size_t)p]) {
                /* Resize the terms array if needed. */
                if (terms_count == terms_capacity) {
                    size_t new_cap = terms_capacity ? terms_capacity * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(prime_list);
                        for (int k = 0; k <= 5; ++k) free(dp[k]);
                        return 0;          /* allocation failure sentinel */
                    }
                    terms = tmp;
                    terms_capacity = new_cap;
                }
                terms[terms_count++] = (seq_t)p;
            }
        }

        /* Clean up temporary allocations. */
        free(prime_list);
        for (int k = 0; k <= 5; ++k) free(dp[k]);

        /* Update the sieve limit for next iteration. */
        last_limit = new_limit;
    }

    /* Return the requested term. */
    return terms[requested - 1];
}
