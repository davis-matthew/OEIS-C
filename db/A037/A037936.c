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
 * Helpers:   _concat_factors, _digits_uint64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/_concat_factors.h"
#include "../utils/_digits_uint64.h"

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
