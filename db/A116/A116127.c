/*
 * A116127  Number of numbers that are congruent to {2, 4} mod 6 between
 *          prime(n) and prime(n+1) inclusive.
 * https://oeis.org/A116127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A116127: Number of integers ≡ 2 or 4 (mod 6) between prime(n)
 * and prime(n+1) inclusive.  Indexing is 1‑based: a(1) is the first term. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"          /* build_primes(idx_t limit,…); */
#include "../utils/_prime_limit_for_idx.h" /* _prime_limit_for_idx(idx_t n); */

static uint64_t *prime_cache = NULL; /* array of primes up to current limit */
static size_t    prime_cnt   = 0;    /* number of primes stored */

/* Counts how many integers ≤ x are ≡ 2 or 4 (mod 6).  */
static seq_t count_leq(uint64_t x)
{
    seq_t cnt = 0;
    if (x >= 2) cnt += (seq_t)((x - 2) / 6) + 1;
    if (x >= 4) cnt += (seq_t)((x - 4) / 6) + 1;
    return cnt;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* safety, though spec says n≥1 */

    /* Ensure we have at least (n+1) primes cached. */
    if (prime_cnt < (size_t)(n + 1)) {
        uint64_t limit = _prime_limit_for_idx((idx_t)(n + 1));

        /* Allocate new array of primes up to 'limit'. */
        uint64_t *tmp   = NULL;
        size_t    tmp_cnt = 0;
        build_primes(limit, (idx_t **)&tmp, &tmp_cnt);

        /* Replace old cache with the new one. */
        free(prime_cache);
        prime_cache = tmp;
        prime_cnt   = tmp_cnt;
    }

    uint64_t L = prime_cache[n - 1]; /* prime(n)  */
    uint64_t R = prime_cache[n];     /* prime(n+1) */

    seq_t cR = count_leq(R);
    seq_t cL = count_leq(L > 0 ? L - 1 : 0);

    return cR - cL;
}
