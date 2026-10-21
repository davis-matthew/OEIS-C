/*
 * A057467  GCD of n-th and (n+1)-st term in the sequence of first differences
 *          between primes, A001223.
 * https://oeis.org/A057467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd( A001223(n+1), A001223(n) ) where A001223(k) =
 *            prime_(k+1) – prime_k (offset 1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   _gcd64, _upper_limit_prime_idx, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_gcd64.h"
#include "../utils/_upper_limit_prime_idx.h"

static uint64_t *prime_cache = NULL; /* cached primes (p_0 = 2, p_1 = 3, …) */
static size_t   prime_count = 0;     /* number of primes currently cached */

/* ------------------------------------------------------------------
 * Ensure that at least `needed` primes are cached (needed ≥ 1).      *
 * If the cache is insufficient, rebuild it with a larger upper     *
 * limit and store the new list, freeing the old one.               *
 * ------------------------------------------------------------------ */
static void _ensure_primes(idx_t needed)   /* needed primes count, 1‑based   */
{
    if (prime_count >= (size_t)needed) return;  /* already sufficient */

    idx_t limit = _upper_limit_prime_idx(needed);
    uint64_t *tmp = NULL;
    size_t   cnt  = 0;

    while (1) {
        build_primes_upto(limit, &tmp, &cnt);
        if (cnt >= (size_t)needed) break;  /* we have enough primes     */
        free(tmp);                         /* not enough – grow the limit */
        limit = limit * 2 + 1;
    }

    free(prime_cache);                /* replace the old cache      */
    prime_cache = tmp;
    prime_count = cnt;
}

/* ------------------------------------------------------------------
 * Main sequence routine: a(n) = gcd( A001223(n+1), A001223(n) ).
 * n is 1‑based; a(1) is defined, a(0) is not used but returns 0.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard */
    /* Need primes p_0 … p_{n+1} → total n+2 primes. */
    _ensure_primes(n + 2);

    size_t idx = (size_t)n;          /* 0‑based index for array access   */
    uint64_t d1 = prime_cache[idx]     - prime_cache[idx - 1];
    uint64_t d2 = prime_cache[idx + 1] - prime_cache[idx];

    uint64_t g = _gcd64(d1, d2);
    return (seq_t)g;                 /* cast to the required seq_t type */
}
