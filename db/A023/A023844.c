/*
 * A023844  Sum of exponents in prime-power factorization of C(5n,n+2).
 * https://oeis.org/A023844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p≤5n} ( vp((5n)!) – vp((n+2)!) – vp((4n – 2)!) ) where
 *            vp(m!) = Σ_{k≥1} ⌊m/p^k⌋. The sum of the exponents in the
 *            prime‑power factorisation of C(5n,n+2)
 * Range:     u8 n <= 671, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Helper: Legendre’s formula for v_p(m!) for a prime p. */
static seq_t v_factorial(idx_t m, idx_t p)
{
    seq_t v = 0;
    idx_t power = p;
    while (power <= m) {
        v += (seq_t)(m / power);
        /* prevent overflow: if power > m / p the next multiplication would
           exceed m or overflow */
        if (power > m / p) {
            break;
        }
        power *= p;
    }
    return v;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* undefined for n < 1, but we return zero */
    }

    /* Cache of all primes up to the current maximum 5*n. */
    static idx_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static idx_t  prime_limit = 0; /* largest 5*n processed so far */

    /* The largest number we need primes for. */
    idx_t limit = 5 * n;
    if (limit > prime_limit) {
        if (prime_arr) {
            free(prime_arr);
        }
        /* build_primes allocates a new array and sets prime_arr and prime_cnt */
        build_primes(limit, &prime_arr, &prime_cnt);
        prime_limit = limit;
    }

    seq_t result = 0;

    /* Loop over all primes ≤ 5*n. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];

        seq_t v5  = v_factorial(5 * n, p);          /* vp((5n)!)   */
        seq_t v1  = v_factorial(n + 2, p);          /* vp((n+2)!) */
        seq_t v2  = v_factorial(4 * n - 2, p);      /* vp((4n-2)!) */

        result += v5 - v1 - v2;
    }
    return result;
}
