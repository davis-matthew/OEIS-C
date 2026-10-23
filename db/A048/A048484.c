/*
 * A048484  a(n) = abs(floor(n/2) - A048299(n)).
 * https://oeis.org/A048484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = | floor(n/2) - A048299(n) |
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes, fact_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A048299(n) is the smallest k that maximises the number of distinct prime divisors of C(n,k). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes.h"   /* provides: void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/fact_exponent.h"

/* local cache for primes */
static bool primes_prepared = false;
static idx_t prime_limit = 0;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 0, a(0) is out of range but return 0 for safety */
        return 0;

    /* Ensure primes up to n are available */
    if (!primes_prepared || n > prime_limit) {
        /* free old list if any */
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
            prime_count = 0;
        }
        build_primes(n, &prime_list, &prime_count);
        prime_limit = n;
        primes_prepared = true;
    }

    /* precompute exponent of each prime in n! */
    idx_t *exp_n = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!exp_n) return 0;              /* out of memory, return 0 */
    for (size_t pi = 0; pi < prime_count; ++pi)
        exp_n[pi] = fact_exponent(n, prime_list[pi]);

    /* search k giving maximal distinct‑prime count */
    size_t best_k = 1;
    size_t best_cnt = 0;

    for (idx_t k = 1; k <= n/2; ++k) {          /* symmetry: C(n,k)=C(n,n-k) */
        size_t cnt = 0;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            idx_t p = prime_list[pi];
            idx_t e = exp_n[pi];
            e -= fact_exponent(k, p);
            e -= fact_exponent(n - k, p);
            if (e > 0)
                ++cnt;
        }
        if (cnt > best_cnt) {
            best_cnt = cnt;
            best_k = k;
        }
    }
    free(exp_n);

    /* dist = |floor(n/2) - best_k|, same for n-k due to symmetry */
    idx_t half = n >> 1;
    seq_t dist = (half >= best_k) ? (seq_t)(half - best_k) : (seq_t)(best_k - half);
    return dist;
}
