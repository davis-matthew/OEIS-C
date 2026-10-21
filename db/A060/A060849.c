/*
 * A060849  Difference between 2 consecutive primes between which a nontrivial
 *          power of prime is found.
 * https://oeis.org/A060849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(A246547(n)) - prevprime(A246547(n))
 * Range:     u8 none, u16 n <= 762, u32 n <= 4321, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, cmp_seq_t, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"

static bool initialized = false;
static seq_t *diffs = NULL;
static size_t diff_cnt = 0;

/* Initialize the cached differences for all prime powers up to a fixed limit.
 * The limit is chosen to exceed the first 10 000 terms comfortably
 * (1 e10 contains more than 2 × 10⁵ prime powers). */
static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    /* Prime generation limit – sqrt of the power limit. */
    const idx_t PRIME_LIMIT = 200000ULL;   /* generates primes up to 200 000 */
    const seq_t POWER_LIMIT = (seq_t)10000000000ULL;   /* 10 000 000 000 */

    /* 1. Generate primes up to PRIME_LIMIT. */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);
    if (!primes) return;

    /* 2. Generate all prime powers p^k (k ≥ 2) ≤ POWER_LIMIT. */
    size_t pow_cap = primes_cnt * 4 + 10;
    seq_t *powers = (seq_t *)malloc(pow_cap * sizeof(seq_t));
    if (!powers) { free(primes); return; }

    size_t pow_cnt = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        unsigned __int128 val = (unsigned __int128)p * p; /* p^2 */

        while (val <= POWER_LIMIT) {
            if (pow_cnt >= pow_cap) {
                pow_cap = pow_cap * 2 + 1;
                seq_t *tmp = (seq_t *)realloc(powers, pow_cap * sizeof(seq_t));
                if (!tmp) { free(powers); free(primes); return; }
                powers = tmp;
            }
            powers[pow_cnt++] = (seq_t)val;
            if (val > POWER_LIMIT / p) break;   /* avoid overflow */
            val *= p;                           /* next exponent */
        }
    }
    free(primes);

    /* 3. Sort the prime powers. */
    qsort(powers, pow_cnt, sizeof(seq_t), cmp_seq_t);

    /* 4. Compute the requested differences between surrounding primes. */
    diffs = (seq_t *)malloc(pow_cnt * sizeof(seq_t));
    if (!diffs) { free(powers); return; }

    for (size_t i = 0; i < pow_cnt; ++i) {
        uint64_t m = (uint64_t)powers[i];
        uint64_t prev = prev_prime(m);
        uint64_t next = next_prime(m);
        diffs[i] = (seq_t)(next - prev);
    }
    diff_cnt = pow_cnt;

    free(powers);   /* powers array no longer needed */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_cache();
    if (!diffs || (size_t)(n - 1) >= diff_cnt) return 0;   /* out of bounds */

    return diffs[(size_t)(n - 1)];
}
