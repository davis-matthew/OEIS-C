/*
 * A220274  a(n) is the smallest number such that for all N >= a(n) there are
 *          at least n primes between 9*N and 10*N.
 * https://oeis.org/A220274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = smallest N such that for all M ≥ N there are at least
 *        n primes between 9·M and 10·M.
 * The implementation precomputes the count of primes in each interval
 * [9k,10k] for k up to a chosen ceiling and stores the tail‑minimum
 * of those counts.  For a query n the smallest index k with that
 * tail‑minimum ≥ n is found by binary search.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum index M that we pre‑compute.
 * Chosen large enough that a(n) for n ≤ 10 000 lies in the range. */
static const idx_t MAXM = 300000ULL;          /* up to 300 000 */
static const idx_t LIM  = 10ULL * MAXM;       /* upper bound for sieve (≈3 M) */

static seq_t *g = NULL;        /* tail‑minimum array g[1..MAXM] */
static bool   init_done = false;

/* construct prime table and tail‑minimum array once */
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* 1. Prime sieve up to LIM */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(LIM + 1));
    if (!is_prime) return;                    /* out of memory → cannot compute */
    for (idx_t i = 0; i <= LIM; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIM; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIM; j += i) is_prime[j] = 0;
        }
    }

    /* 2. Prefix prime counts π(x) */
    uint32_t *pi = (uint32_t*)malloc((size_t)(LIM + 1) * sizeof(uint32_t));
    if (!pi) { free(is_prime); return; }
    pi[0] = 0;
    for (idx_t i = 1; i <= LIM; ++i) 
        pi[i] = pi[i-1] + (uint32_t)is_prime[i];

    /* 3. Allocate g array */
    g = (seq_t*)malloc((size_t)(MAXM + 2) * sizeof(seq_t));
    if (!g) { free(is_prime); free(pi); return; }

    /* 4. Compute f(k) = π(10k)-π(9k-1) and tail minima */
    for (idx_t k = 1; k <= MAXM; ++k) {
        idx_t low  = 9ULL * k;
        idx_t high = 10ULL * k;
        seq_t f = (seq_t)(pi[high] - pi[low - 1]);
        g[k] = f;
    }
    for (idx_t k = MAXM; k >= 1; --k) {
        if (k < MAXM && g[k] > g[k+1]) g[k] = g[k+1];
    }

    /* 5. Clean up temporary arrays */
    free(is_prime);
    free(pi);
}

/* requested function */
seq_t a(idx_t n)
{
    init();
    if (!g || n == 0) return 0;

    /* binary search for smallest M with g[M] ≥ n */
    idx_t low = 1, high = MAXM;
    seq_t sn = (seq_t)n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (g[mid] >= sn) high = mid;
        else              low  = mid + 1;
    }
    return (seq_t)low;
}
