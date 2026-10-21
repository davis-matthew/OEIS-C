/*
 * A088183  Number of ways to write n as a sum of two coprime semiprimes.
 * https://oeis.org/A088183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 720, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n as a sum of two coprime semiprimes.
   Implementation uses a sieve of primes up to n, marks all semiprimes,
   and then counts unordered coprime pairs (a,b) with a+b==n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    /* primes up to the current max_n, stored as 64‑bit integers. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    /* flags for semiprimes up to max_n. */
    static bool *is_semi = NULL;

    /* Rebuild tables if we have seen a larger n. */
    if (n > max_n) {
        free(is_semi);
        free(primes);

        /* Get all primes up to n. */
        build_primes_upto(n, &primes, &primes_cnt);

        /* Allocate and clear flag array. */
        is_semi = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
        for (size_t i = 0; i <= (size_t)n; ++i)
            is_semi[i] = false;

        /* Mark all semiprimes <= n. */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            for (size_t j = i; j < primes_cnt; ++j) {
                uint64_t q = primes[j];
                idx_t prod = (idx_t)p * (idx_t)q;
                if (prod > n)
                    break;
                is_semi[prod] = true;
            }
        }

        max_n = n;
    }

    seq_t count = 0;
    /* Count unordered coprime semiprime pairs a <= b with a + b = n. */
    for (idx_t a_val = 4; a_val <= n / 2; ++a_val) {
        if (!is_semi[a_val])
            continue;
        idx_t b_val = n - a_val;
        if (b_val < a_val)
            continue;    /* ensure a <= b */
        if (!is_semi[b_val])
            continue;
        if (gcd((seq_t)a_val, (seq_t)b_val) == (seq_t)1)
            ++count;
    }

    return count;
}
