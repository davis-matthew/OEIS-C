/*
 * A073461  Largest solution to phi(x) - pi(x) = n or 0 if no such solution
 *          exists.
 * https://oeis.org/A073461
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Largest x such that φ(x)-π(x)=n, or 0 if none
 * Range:     u8 n <= 13, u16 n <= 6764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 1000000U            /* sieve upper bound (enough for n ≤ 10000) */
#define MAX_N 10000              /* maximum n to support (inclusive) */

/* cache for a(n), indexed by n+2 (so n=-2 maps to index 0) */
static seq_t a_cache[MAX_N + 3] = {0};
static bool initialized = false;

/* initialise cache on first call */
static void init_cache(void) {
    if (initialized) return;

    const size_t N = LIMIT;
    /* temporary arrays for sieve */
    uint32_t *phi = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *pi  = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *lp  = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!phi || !pi || !lp) { abort(); }

    /* reserve space for prime list */
    size_t prime_cap = N / 10 + 32;
    size_t *primes = (size_t *)malloc(prime_cap * sizeof(size_t));
    if (!primes) { abort(); }
    size_t pc = 0;

    /* initialise base values */
    phi[0] = 0; phi[1] = 1;
    pi[0]  = 0; pi[1]  = 0;
    lp[0]  = 0; lp[1]  = 0;

    /* linear sieve */
    for (size_t i = 2; i <= N; ++i) {
        if (lp[i] == 0) {                /* i is prime */
            lp[i] = (uint32_t)i;
            phi[i] = (uint32_t)i - 1;
            primes[pc++] = i;
        }
        pi[i] = pi[i - 1] + ((lp[i] == (uint32_t)i) ? 1 : 0);

        for (size_t j = 0; j < pc; ++j) {
            size_t p   = primes[j];
            size_t ip  = i * p;
            if (ip > N) break;
            lp[ip] = (uint32_t)p;
            if (i % p == 0) {
                phi[ip] = phi[i] * (uint32_t)p;
                break;
            } else {
                phi[ip] = phi[i] * (uint32_t)(p - 1);
            }
        }
    }

    /* populate cache: largest x for each diff in [-2 .. MAX_N] */
    for (size_t i = 1; i <= N; ++i) {
        int diff = (int)((int32_t)phi[i] - (int32_t)pi[i]);  /* safe cast */
        if (diff >= -2 && diff <= MAX_N) {
            seq_t cand = (seq_t)i;
            if (cand > a_cache[diff + 2]) {
                a_cache[diff + 2] = cand;
            }
        }
    }

    /* free temporaries */
    free(phi);
    free(pi);
    free(lp);
    free(primes);

    initialized = true;
}

/* required function */
seq_t a(idx_t n) {
    if (!initialized) init_cache();

    if (n < -2) return 0;
    if (n > MAX_N) return 0;     /* out of supported range -> no solution */
    return a_cache[n + 2];
}
