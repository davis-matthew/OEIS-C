/*
 * A210502  a(1) = 1, a(n) = A210501(n) - 1.
 * https://oeis.org/A210502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k odd | φ(k) = prime(n)-1} - [prime(n) > 2]
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static idx_t phi_limit = 0;          /* highest index of precomputed φ */
static idx_t max_target = 0;         /* largest target = prime(n)-1 processed */
static idx_t *phi_arr = NULL;        /* φ values (size = phi_limit+1) */
static seq_t *counts  = NULL;        /* counts[ t ] = #odd k with φ(k)=t (t≤max_target) */
static bool initialized = false;

/* Precompute φ(k) for 0≤k≤limit and counts for all targets up to target_max. */
static void compute_phi_and_counts(idx_t limit, idx_t target_max)
{
    /* Allocate and fill φ array */
    phi_arr = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!phi_arr) abort();
    for (idx_t i = 0; i <= limit; ++i) phi_arr[i] = i;
    if (limit >= 1) phi_arr[1] = 1;

    /* Euler's totient sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi_arr[i] == i) {                /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }

    /* Allocate counts array */
    counts = (seq_t *)calloc((size_t)(target_max + 1), sizeof(seq_t));
    if (!counts) abort();

    /* Count odd k with φ(k) = t (t≤target_max) */
    for (idx_t k = 1; k <= limit; ++k) {
        if ((k & 1) == 1) {                   /* k is odd */
            idx_t t = phi_arr[k];
            if (t <= target_max)
                counts[t] += (seq_t)1;
        }
    }

    phi_limit  = limit;
    max_target = target_max;
}

/* Extend computation to cover a larger maximum target (prime(n)-1). */
static void extend_to_target(idx_t new_target)
{
    if (new_target <= max_target) return;

    /* New limits – a bit larger than 6×target to be safe. */
    idx_t new_limit = 6 * new_target + 10;
    if (new_limit < phi_limit) new_limit = phi_limit;

    /* Free old arrays and recompute everything at once. */
    free(phi_arr);
    free(counts);
    compute_phi_and_counts(new_limit, new_target);
}

/* One‑time initialization for a(n) up to index 10,000. */
static void init_once(void)
{
    if (initialized) return;

    const idx_t N_LIMIT = 10000;                       /* maximum n needed for tests */
    idx_t target_max = (idx_t)get_prime(N_LIMIT) - 1;  /* prime(N_LIMIT)-1 */
    if (target_max < (idx_t)1) target_max = 1;        /* safety */

    idx_t limit = 6 * target_max + 10;
    compute_phi_and_counts(limit, target_max);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset, safe guard */

    init_once();                          /* compute up to at least 10,000 */

    seq_t p = (seq_t)get_prime(n);
    idx_t target = (idx_t)(p - (seq_t)1);

    if (target > max_target)
        extend_to_target(target);         /* rare; handles larger n if needed */

    seq_t res = counts[target];
    if (p > (seq_t)2)                      /* exclude k = prime(n) when odd */
        res = res - (seq_t)1;

    return res;
}
