/*
 * A073462  Number of solutions to phi(x) - pi(x) = n.
 * https://oeis.org/A073462
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of solutions to phi(x) - pi(x) = n
   a(n) = |{x : phi(x) - pi(x) = n}|
   Offset: -1
   The function precomputes φ and π for all x up to a fixed bound
   sufficient to cover all n ≤ 10000.  The value is then looked up
   in an array in O(1).  The algorithm is linear-time sieve for φ
   combined with a running count of primes.  It runs well under 2 s
   for every n in [−1,10000].  For n < −1 or n > 10000 the
   function returns 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The caller-provided types */
extern seq_t calc_phi(seq_t m);   /* not used – φ is computed inline */
extern seq_t compute_pi(idx_t n); /* not used – π is computed inline */

#define MAX_N    10000                     /* maximum n that we care about */
#define LIMIT    2000000                   /* sieve bound – enough for all n≤10000 */

static seq_t *counts = NULL;            /* counts[n+1] for n in [-1,MAX_N] */
static bool initialized = false;

/* Linear sieve that also counts the number of solutions for
   φ(x)-π(x) = n for n∈[-1,MAX_N].  */
static void init_counts(void)
{
    /* Allocate φ array */
    seq_t *phi = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!phi) return; /* allocation failure – ignore for brevity */

    /* Sieve data structures */
    uint8_t *is_composite = (uint8_t *)calloc(LIMIT + 1, 1);
    uint32_t *prime_buf   = (uint32_t *)malloc((LIMIT / 10 + 100) * sizeof(uint32_t));
    if (!is_composite || !prime_buf) {
        free(phi); free(is_composite); free(prime_buf);
        return;
    }

    size_t prime_cnt = 0;
    seq_t pi_val = 0;    /* current number of primes ≤ i */

    /* initialise counts array */
    counts = (seq_t *)malloc((MAX_N + 2) * sizeof(seq_t));
    if (!counts) { free(phi); free(is_composite); free(prime_buf); return; }
    for (int i = 0; i < MAX_N + 2; ++i) counts[i] = 0;

    /* Treat x = 1 separately (φ(1)=1, π(1)=0) */
    { seq_t diff = (seq_t)1; /* φ(1)-π(1)=1 */
      if ((int64_t)diff >= -1 && (int64_t)diff <= MAX_N)
          counts[ (size_t)(diff + 1) ]++; }

    /* Initialize φ[1] */
    phi[1] = 1;
    is_composite[0] = is_composite[1] = 1; /* not primes */

    /* Main sieve loop */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            prime_buf[prime_cnt++] = (uint32_t)i;
            phi[i] = (seq_t)(i - 1);  /* φ(p) = p-1 for primes */
            ++pi_val;                 /* i is prime */
        } else {
            /* φ[i] already computed in the inner loop below */
        }

        /* Update φ for multiples of i via linear sieve */
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_buf[j];
            idx_t idx = i * (idx_t)p;
            if (idx > LIMIT) break;
            is_composite[idx] = 1;
            if (i % p == 0) {
                phi[idx] = phi[i] * (seq_t)p;
                break;
            } else {
                phi[idx] = phi[i] * (seq_t)(p - 1);
            }
        }

        /* Count solutions for current i */
        seq_t diff = phi[i] - pi_val;
        if ((int64_t)diff >= -1 && (int64_t)diff <= MAX_N)
            counts[(size_t)(diff + 1)]++;
    }

    /* Clean up */
    free(phi);
    free(is_composite);
    free(prime_buf);
    initialized = true;
}

/* Public API */
seq_t a(idx_t n)
{
    if (!initialized) init_counts();
    if (n < -1 || n > MAX_N) return 0;
    return counts[(size_t)(n + 1)];
}
