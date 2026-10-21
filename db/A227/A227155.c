/*
 * A227155  Number of composites removed in each step of the Sieve of
 *          Eratosthenes for 10^7.
 * https://oeis.org/A227155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = number of composites ≤10⁷ whose least prime * divisor is
 *            the n‑th prime. The values are precomputed by a * single sieve
 *            of Eratosthenes up to 10⁷. For n beyond the last prime, * a(n)=0
 * Range:     u8 none, u16 none, u32 n <= 446, u64 n <= 446, u128 n <= 446,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Closed upper limit used in the definition of the sequence. */
static const idx_t N_CONST = 10000000ULL;

/* Allocation limits.  π(10⁷)=664579, so 664600 is a safe upper bound. */
#define MAX_PRIME_COUNT 664600

/* Static storage for the precomputed tables.  They are initialized
 * lazily on the first call to a(). */
static uint32_t *primes = NULL;          /* primes[i] = i‑th prime (0‑based)   */
static seq_t   *counts = NULL;           /* counts[i] = composites w/ lpf = primes[i] */
static uint32_t *lpf   = NULL;           /* least prime factor of each number   */
static idx_t    prime_count = 0;         /* actual number of primes found      */

/* Single‑time sieve initialisation.  The routine builds three tables:
 *
 *   1.  `primes` – the list of all primes ≤N_CONST.
 *   2.  `counts` – for each prime, the number of composites that
 *      have this prime as their least prime divisor.
 *   3.  `lpf`   – the least prime factor of each integer ≤N_CONST,
 *      used to avoid double‑counting numbers.
 *
 * The algorithm runs in Θ(N) time and uses ≈48 MB of memory. */
static void init_sieve(void)
{
    if (prime_count) return; /* already initialized */

    /* Allocate the tables.  The bounds are chosen conservatively. */
    primes = (uint32_t *)malloc(MAX_PRIME_COUNT * sizeof(uint32_t));
    counts = (seq_t   *)malloc(MAX_PRIME_COUNT * sizeof(seq_t));
    lpf    = (uint32_t *)calloc((size_t)(N_CONST + 1), sizeof(uint32_t));
    if (!primes || !counts || !lpf) {
        /* In a production environment you would handle this more gracefully. */
        abort();
    }

    /* The sieve.  We iterate over i from 2 to N_CONST, marking each prime
     * and building the composites' counts as we go.  The inner loop
     * processes all multiples of a found prime. */
    for (idx_t i = 2; i <= N_CONST; ++i) {
        if (lpf[i] == 0) {                     /* i is prime */
            lpf[i] = (uint32_t)i;              /* record its own LFp */
            uint32_t idx = prime_count;        /* index of this prime */
            primes[prime_count++] = (uint32_t)i;
            counts[idx] = 0;                   /* initialise counter */

            /* For all multiples of this prime that are still unmarked,
             * set their Lfp and increase the counter.  The loop starts
             * at 2*i to avoid counting the prime itself. */
            if (i <= N_CONST / 2) {
                for (idx_t j = i + i; j <= N_CONST; j += i) {
                    if (lpf[j] == 0) {
                        lpf[j] = (uint32_t)i;
                        counts[idx]++;          /* j has least prime = i */
                    }
                }
            }
        }
    }

    /* The arrays primes[] and counts[] now have length prime_count.
     * The remaining space (if any) is unused. */
}

/* --------------------------------------------------------------------- */
/*  Public API: compute the n‑th term of the sequence.                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sieve();                 /* ensure tables are ready */

    if (n < 1) return 0;          /* sequence is 1‑based; below offset is unspecified */

    if ((idx_t)(n - 1) >= prime_count) return 0; /* primes exhausted */

    return counts[n - 1];
}
