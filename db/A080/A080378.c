/*
 * A080378  Residues mod 4 of the n-th difference between consecutive primes.
 * https://oeis.org/A080378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1) - prime(n)) mod 4 == A001223(n) mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* n is 1‑based. Return (p_{n+1} - p_n) mod 4. */
    if (n == 0) return (seq_t)0;                /* not part of OEIS offset */
    idx_t target = n + 1;                       /* we need (n+1)-th prime */
    idx_t lim = _prime_limit_for_idx(target);   /* upper bound for that prime */

    uint64_t *primes = NULL;
    size_t count = 0;
    build_primes_upto(lim, &primes, &count);

    /* In rare cases the bound may be too low; expand until we have enough primes. */
    while (count < (size_t)target) {
        free(primes);
        lim *= 2;                               /* safety factor */
        build_primes_upto(lim, &primes, &count);
    }

    seq_t diff = (seq_t)(primes[target-1]) - (seq_t)(primes[target-2]);
    free(primes);
    return diff % 4;
}
