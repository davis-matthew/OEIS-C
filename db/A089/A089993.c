/*
 * A089993  Penultimate prime divisor of numbers that are not powers of primes
 *          (A024619).
 * https://oeis.org/A089993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = penultimate prime divisor of the nth number that is not a
 *            prime power (i.e., a(n) = A076820(A024619(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* not defined for n < 1 */
    idx_t cnt = 0;                   /* number of non‑prime‑power terms found */
    uint64_t m = 2;                  /* current integer to test */

    for (;;) {
        /* skip prime powers: numbers with only one distinct prime factor */
        if (count_distinct_primes(m) >= 2) {
            ++cnt;
            if ((idx_t)cnt == n) {
                /* factor m to obtain distinct prime factors */
                uint64_t primes[64];
                uint64_t exps[64];
                size_t pfCnt;
                factor_u64(m, primes, exps, &pfCnt);
                /* pfCnt ≥ 2 by construction – second largest is at index pfCnt-2 */
                return (seq_t)primes[pfCnt - 2];
            }
        }
        ++m;                          /* next integer */
    }
}
