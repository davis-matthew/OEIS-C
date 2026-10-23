/*
 * A020643  a(n)-th squarefree is sum of first k squarefrees for some k.
 * https://oeis.org/A020643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the index (in the list of squarefree numbers) of a
 *            sum of consecutive squarefree numbers that itself is squarefree
 * Range:     u8 n <= 11, u16 n <= 230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"
#include "../utils/_floor_isqrt_u64.h"

/* --------------------------------------------------------------------
 *  Möbius function precomputation up to MU_LIMIT.
 *  Used to count squarefree numbers with the formula:
 *      sf(x) = Σ_{d=1}^{⌊√x⌋} μ(d) * ⌊x / d²⌋
 *  Where μ(d) is the Möbius function.
 */
#define MU_LIMIT 200000

static int mu[MU_LIMIT + 1];
static bool mu_computed = false;

static void compute_mu(void)
{
    if (mu_computed) return;
    mu_computed = true;

    static bool is_comp[MU_LIMIT + 1];
    static int primes[MU_LIMIT];
    int pc = 0;

    mu[1] = 1;
    for (idx_t i = 2; i <= MU_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            mu[i] = -1;
        }
        for (int j = 0; j < pc && (idx_t)primes[j] * i <= MU_LIMIT; ++j) {
            idx_t ip = (idx_t)primes[j] * i;
            is_comp[ip] = true;
            if (i % primes[j] == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
}

/* --------------------------------------------------------------------
 *  Small squarefree table for fast look‑ups of numbers ≤ SMALL_LIMIT.
 */
#define SMALL_LIMIT 500000

static unsigned char *small_sf = NULL;   /* 0 = not squarefree, 1 = squarefree */

static void init_small_sf(void)
{
    if (small_sf) return;
    small_sf = (unsigned char *)malloc((size_t)SMALL_LIMIT + 1);
    if (!small_sf) return;   /* allocation failure – unlikely */

    for (size_t i = 0; i <= (size_t)SMALL_LIMIT; ++i) small_sf[i] = 1;
    small_sf[0] = 0;  /* 0 has squared factors */

    /* Mark numbers that have a square divisor */
    for (idx_t p = 2; p * p <= SMALL_LIMIT; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= SMALL_LIMIT; j += sq)
            small_sf[j] = 0;
    }
}

/* --------------------------------------------------------------------
 *  Count of squarefree integers ≤ x (x assumed to fit in 64‑bit).
 */
static idx_t count_squarefree_upto(seq_t x)
{
    if (x == 0) return 0;
    idx_t xt = (idx_t)x;
    idx_t r   = _floor_isqrt_u64(xt);
    idx_t sum = 0;

    for (idx_t d = 1; d <= r; ++d) {
        if (mu[d] == 0) continue;
        idx_t term = xt / ((idx_t)d * (idx_t)d);
        sum += (idx_t)mu[d] * term;
    }
    return sum;
}

/* --------------------------------------------------------------------
 *  Main routine: returns a(n) for arbitrary n ≥ 1, caching previous results.
 */
seq_t a(idx_t n)
{
    /* --- static initialization ----------------------------------- */
    static bool initialized = false;
    if (!initialized) {
        init_small_sf();
        compute_mu();
        initialized = true;
    }

    /* --- dynamic cache of the sequence values -------------------- */
    static seq_t *cache  = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* --- state of the forward scan over the natural numbers ---- */
    static idx_t current_int = 0;          /* last integer examined */
    static seq_t cumulative_sum  = 0;      /* sum of all squarefrees seen */

    /* --- generate terms until we have at least n entries ------------ */
    while (cache_len < (size_t)n) {
        /* Find next squarefree integer. */
        while (true) {
            ++current_int;
            bool sf;
            if (current_int <= SMALL_LIMIT)
                sf = small_sf[current_int];
            else
                sf = is_squarefree((seq_t)current_int);
            if (sf) break;
        }

        /* Add this squarefree to the running sum. */
        cumulative_sum += (seq_t)current_int;

        /* Test whether the new sum is squarefree. */
        bool sum_sf;
        if (cumulative_sum <= (seq_t)SMALL_LIMIT)
            sum_sf = small_sf[(size_t)cumulative_sum];
        else
            sum_sf = is_squarefree(cumulative_sum);

        if (sum_sf) {
            /* Determine the index of this sum among all squarefrees. */
            idx_t idx = count_squarefree_upto(cumulative_sum);

            /* Grow the cache if needed. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)idx;
        }
    }

    return cache[n - 1];
}
