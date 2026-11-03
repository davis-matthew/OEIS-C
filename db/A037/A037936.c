/*
 * A037936  Trajectory of 38 under prime factor concatenation procedure.
 * https://oeis.org/A037936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, tabf, base
 * Formula:   trajectory of 38 under prime factor concatenation procedure.
 *            a(0) = 38 for n ≥ 0: if a(n) is composite, write its prime
 *            factors (with multiplicity) in ascending order and append them
 *            to the sequence. Next composite = concatenation of those prime
 *            factors. If a(n) is prime, append it and stop
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 6 terms through n = 5 (2026-11-02)
 * Helpers:   factor_u64, num_digits_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits_u64_to_u64.h"

static seq_t _concat_factors(const uint64_t *pr, const uint64_t *ex, size_t cnt) {
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (uint64_t m = 0; m < ex[i]; ++m) {
            uint64_t p = pr[i];
            uint64_t pow10 = 1;
            uint64_t temp = p;
            while (temp > 0) {
                temp /= 10;
                pow10 *= 10;
            }
            result = result * pow10 + p;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* Static cache of the trajectory */
    static seq_t seq[64];
    static idx_t len = 0;
    if (len == 0) {
        /* Build the trajectory once */
        seq[0] = (seq_t)38;
        idx_t pos = 1;
        seq_t cur = seq[0];
        while (true) {
            /* factorize current (assumed to fit in uint64_t) */
            uint64_t cur64 = (uint64_t)cur;
            uint64_t primes[32], exps[32];
            size_t cnt = 0;
            factor_u64(cur64, primes, exps, &cnt);
            /* Append prime factors (with multiplicity) */
            for (size_t i = 0; i < cnt; ++i) {
                for (uint64_t k = 0; k < exps[i]; ++k) {
                    seq[pos++] = (seq_t)primes[i];
                }
            }
            /* Detect if cur is prime (single factor of itself) */
            if (cnt == 1 && exps[0] == 1 && primes[0] == cur64) {
                break;  /* stop after appending the prime */
            }
            /* Compute next composite by concatenating the factors */
            cur = _concat_factors(primes, exps, cnt);
        }
        len = pos;
    }

    if (n < len) return seq[n];
    return 0;  /* undefined beyond known trajectory */
}
