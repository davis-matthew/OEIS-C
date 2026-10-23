/*
 * A129926  Semiprimes s such that 3*s - 2 is a prime.
 * https://oeis.org/A129926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 941, u32 n <= 942, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for semiprimes s such that 3*s-2 is a prime. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 4;          /* start checking from 4 */

    /* Small buffers for the factor_u64 helper. */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt;

    /* Return a value we already know if enough terms have been
       found previously. */
    if ((idx_t)terms_len >= n) {
        return terms[n - 1];
    }

    /* Find the next terms until we have at least n terms stored. */
    while ((idx_t)terms_len < n) {
        uint64_t s = (uint64_t)next_candidate;

        /* Search for the next semiprime that satisfies the property. */
        while (1) {
            factor_u64(s, pr, ex, &cnt);

            /* Count the total multiplicity of prime factors. */
            size_t mult = 0;
            for (size_t i = 0; i < cnt; ++i) {
                mult += ex[i];
            }

            if (mult == 2) {                      /* s is a semiprime */
                uint64_t m = 3ULL * s - 2ULL;
                if (is_prime_big((seq_t)m)) {     /* 3*s-2 is prime */
                    append_candidate(&terms, &terms_len, &terms_cap,
                                     (seq_t)s);
                    break;                       /* found next term */
                }
            }
            ++s;                                  /* try next integer */
        }

        /* All future searches start after the one we just found. */
        next_candidate = s + 1;
    }

    return terms[n - 1];
}
