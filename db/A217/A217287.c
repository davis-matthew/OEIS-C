/*
 * A217287  Length of chain of consecutive integers starting with n, where
 *          each new integer in the chain has a prime factor which no previous
 *          member in the chain has.
 * https://oeis.org/A217287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of chain of consecutive integers starting at n,
 *            where each new integer has at least one prime factor not seen
 *            before
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* outside OEIS offset */

    /* Buffer for used primes — the chain never uses many different primes,
       so a modest fixed size is sufficient.          */
    const size_t USED_MAX = 1024;
    uint64_t used[USED_MAX];
    size_t used_cnt = 0;

    /* Prepare factor arrays: factor_u64 returns distinct primes in `pr`. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t fact_cnt = 0;

    /* Start chain with n itself */
    seq_t length = 1;

    /* Add primes of n to the used set (if n > 1). */
    factor_u64((uint64_t)n, pr, ex, &fact_cnt);
    for (size_t i = 0; i < fact_cnt; ++i) {
        uint64_t p = pr[i];
        bool present = false;
        for (size_t j = 0; j < used_cnt; ++j)
            if (used[j] == p) { present = true; break; }
        if (!present) {
            if (used_cnt < USED_MAX) used[used_cnt++] = p;
        }
    }

    /* Iterate over successive integers until a number has no new prime factor. */
    idx_t m = n;
    while (true) {
        if (m == UINT64_MAX) break;            /* avoid overflow */
        ++m;
        factor_u64((uint64_t)m, pr, ex, &fact_cnt);

        /* Check whether this integer introduces a new prime factor. */
        bool has_new = false;
        for (size_t i = 0; i < fact_cnt; ++i) {
            uint64_t p = pr[i];
            bool present = false;
            for (size_t j = 0; j < used_cnt; ++j)
                if (used[j] == p) { present = true; break; }
            if (!present) { has_new = true; break; }
        }

        if (!has_new) break;                      /* chain terminates */

        /* Record all prime factors of this integer. */
        for (size_t i = 0; i < fact_cnt; ++i) {
            uint64_t p = pr[i];
            bool present = false;
            for (size_t j = 0; j < used_cnt; ++j)
                if (used[j] == p) { present = true; break; }
            if (!present) {
                if (used_cnt < USED_MAX) used[used_cnt++] = p;
            }
        }

        ++length;                                 /* include this integer */
    }

    return length;
}
