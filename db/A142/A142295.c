/*
 * A142295  Primes congruent to 7 mod 44.
 * https://oeis.org/A142295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 320, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142295: primes congruent to 7 (mod 44) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* a(n) = the n-th prime p such that p % 44 == 7   (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but safe return */

    /* Rough upper bound for the n-th prime in the progression:
       since primes in this class have density 1/φ(44)=1/20,
       the n-th such prime is < 20*n*log(20*n).  The function
       _prime_limit_for_idx gives a safe upper bound for the
       m-th regular prime.  Scale m up to 20*n+100 to be safe. */
    idx_t m = n * 20ULL + 100ULL;
    if (m == 0) m = 1;

    idx_t limit = _prime_limit_for_idx(m);

    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(limit, &primes, &primes_cnt);

    idx_t found = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (primes[i] % 44ULL == 7ULL) {
            ++found;
            if (found == n) {
                seq_t result = (seq_t)primes[i];
                free(primes);
                return result;
            }
        }
    }

    /* If we reach here, something went wrong; return 0. */
    free(primes);
    return 0;
}
