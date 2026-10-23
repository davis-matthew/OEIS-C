/*
 * A018729  Divisors of 932.
 * https://oeis.org/A018729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018729: Divisors of 932 sorted in ascending order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static cache of divisors of 932 */
    static seq_t divs[64];
    static size_t divcnt = 0;
    static bool initialized = false;

    if (!initialized) {
        /* factor 932 = 2^2 * 233^1 */
        const uint64_t N = 932ULL;
        uint64_t primes[16];
        uint64_t exps[16];
        size_t pfcnt = 0;
        factor_u64(N, primes, exps, &pfcnt);

        /* generate all divisors */
        size_t curcnt = 1;
        divs[0] = (seq_t)1;

        for (size_t pi = 0; pi < pfcnt; ++pi) {
            uint64_t p = primes[pi];
            uint64_t e = exps[pi];
            size_t oldcnt = curcnt;
            for (size_t j = 0; j < oldcnt; ++j) {
                seq_t base = divs[j];
                seq_t val = base;
                for (uint64_t k = 1; k <= e; ++k) {
                    val *= (seq_t)p;            /* multiply by prime */
                    divs[curcnt++] = val;
                }
            }
        }
        divcnt = curcnt;

        /* sort divisors in ascending order (in‑place insertion sort) */
        for (size_t i = 1; i < divcnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        initialized = true;
    }

    if (n < 1 || (idx_t)n > divcnt)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
