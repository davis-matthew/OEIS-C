/*
 * A178133  Number of odd semiprimes between consecutive squares.
 * https://oeis.org/A178133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd squarefree semiprimes s with n^2 < s <
 *            (n+1)^2
 * Range:     u8 n <= 695, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* The types seq_t and idx_t are supplied by the build environment. */
/* extern idx_t; extern seq_t; are not required to be declared here. */

/* ------------------------------------------------------------------------------ */
/* Static configuration ----------------------------------------------------------- */
static const idx_t MAX_N = 10000ULL;          /* maximum index we precompute */
static bool initialized = false;              /* one–time initialization flag */
static seq_t *counts = NULL;                  /* array of a(n) values */

/* ------------------------------------------------------------------------------ */
/* Utility: integer square root (floor) for 64‑bit values ------------------------ */
static inline idx_t isqrt_u64(idx_t x)
{
    /* Use a double approximation and correct by a few steps.           */
    idx_t r = (idx_t)sqrt((double)x);          /* approximate */
    while ((r + 1ULL) * (r + 1ULL) <= x) ++r;  /* adjust upward   */
    while (r * r > x) --r;                    /* adjust downward */
    return r;
}

/* ------------------------------------------------------------------------------ */
/* One‑time precomputation of all a(n) up to MAX_N ------------------------------ */
static void init_counts(void)
{
    /* Upper bound for the products of two odd primes needed for intervals
       up to MAX_N.                                       */
    idx_t limit = (MAX_N + 1ULL) * (MAX_N + 1ULL);  /* (MAX_N+1)^2          */
    /* --------------------------------------------------------- */
    /* Step 1: Sieve of Eratosthenes to find all primes ≤ limit. */
    /* --------------------------------------------------------- */
    size_t sieve_size = (size_t)(limit + 1ULL);
    bool *is_prime = (bool *)calloc(sieve_size, sizeof(bool));
    if (!is_prime) return;          /* allocation failed – abort silently */

    /* Initialise all entries to true, then clear 0 and 1.                  */
    for (size_t i = 0; i < sieve_size; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Treat even numbers >2 as composite.                                 */
    for (size_t i = 4; i < sieve_size; i += 2) is_prime[i] = false;

    /* Sieve odd numbers only.                                             */
    size_t sqrt_lim = (size_t)isqrt_u64(limit);
    for (size_t i = 3; i <= sqrt_lim; i += 2) {
        if (is_prime[i]) {
            /* Mark composites of this prime.  Step is 2*i to stay on odd.   */
            for (size_t j = i * i; j < sieve_size; j += i * 2) {
                is_prime[j] = false;
            }
        }
    }

    /* Collect odd primes (2 is skipped as it would produce even semiprimes). */
    idx_t *prime_buf = (idx_t *)malloc(sieve_size * sizeof(idx_t));
    if (!prime_buf) { free(is_prime); return; }

    size_t prime_cnt = 0;
    for (size_t i = 3; i < sieve_size; i += 2) {
        if (is_prime[i]) {
            prime_buf[prime_cnt++] = (idx_t)i;
        }
    }

    /* ----------------------------------------------------- */
    /* Step 2: Allocate array for a(n) values.              */
    /* ----------------------------------------------------- */
    counts = (seq_t *)calloc(MAX_N + 1ULL, sizeof(seq_t));
    if (!counts) {
        free(is_prime);
        free(prime_buf);
        return;
    }

    /* ----------------------------------------------------- */
    /* Step 3: Generate all odd squarefree semiprimes ≤ limit. */
    /* For each such semiprime s, increment the count
       for the interval floor(sqrt(s)).                       */
    /* ----------------------------------------------------- */
    idx_t sqrt_limit = (idx_t)isqrt_u64(limit);

    for (size_t ip = 0; ip < prime_cnt; ++ip) {
        idx_t p = prime_buf[ip];
        if (p > sqrt_limit) break;          /* no further pair has product ≤ limit */

        for (size_t iq = ip + 1; iq < prime_cnt; ++iq) {
            idx_t q = prime_buf[iq];
            idx_t prod = p * q;
            if (prod > limit) break;        /* products will only grow larger */

            idx_t k = isqrt_u64(prod);     /* k satisfies k^2 ≤ prod < (k+1)^2 */
            if (k >= 1ULL && k <= MAX_N) {
                counts[k] += (seq_t)1;   /* increment a(k) */
            }
        }
    }

    /* ----------------------------------------------------- */
    /* Cleanup temporary data structures.                   */
    /* ----------------------------------------------------- */
    free(is_prime);
    free(prime_buf);
    initialized = true;
}

/* ------------------------------------------------------------------------------ */
/* Public sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_counts();
    if (n == 0ULL || n > MAX_N) return (seq_t)0;
    return counts[n];
}
