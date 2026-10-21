/*
 * A231367  a(n) = 1 if A024816(m) = n has a solution for some m, where
 *          A024816(m) = sums of non-divisors of m = antisigma(m), otherwise
 *          0.
 * https://oeis.org/A231367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists m such that A024816(m) = n
 *            (antisigma(m)), otherwise 0 (a(n) = 1 iff n is an attainable
 *            antisigma value). The routine pre‑computes antisigma(m) for all
 *            m ≤ 5000 and records which n values occur. For any n beyond the
 *            table size it returns 0. This covers all n needed for th...
 * Range:     u8 none, u16 n <= 21, u32 n <= 9077, u64 n <= 9077,
 *            u128 n <= 9077, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow_uint.h"

/* Constant: maximal m for which antisigma values are precomputed. */
static const idx_t LIMIT    = 5000;

/* Memoization state. */
static bool   *visited        = NULL;  /* visited[n] = true iff n attainable */
static size_t  visited_size   = 0;

static uint64_t *spf          = NULL; /* smallest prime factor array   */
static size_t   spf_size      = 0;

/* Build smallest prime factor array up to n (inclusive). */
static void build_spf(idx_t n)
{
    if (spf_size > n) return;

    spf = (uint64_t *)realloc(spf, sizeof(uint64_t) * (n + 1));
    memset(spf + spf_size, 0, (n - spf_size + 1) * sizeof(uint64_t));

    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
    spf_size = n + 1;
}

/* Compute sigma(m) using the pre‑computed spf array. */
static seq_t sigma_of_m(idx_t m)
{
    seq_t sigma = 1;
    idx_t t = m;
    while (t > 1) {
        uint64_t p = spf[t];
        idx_t exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        seq_t term = pow_uint(p, exp + 1);               /* p^(exp+1)   */
        term -= 1;                                      /* (p^(exp+1)-1) */
        term /= (seq_t)(p - 1);                         /* / (p-1)     */
        sigma *= term;
    }
    return sigma;
}

/* Initialise the lookup tables: Allocate visited array and fill it with
   antisigma values for m up to LIMIT. */
static void initialise(void)
{
    /* Number of possible antisigma values for m up to LIMIT. */
    visited_size = (LIMIT * (LIMIT + 1)) / 2 + 1;
    visited = (bool *)malloc(visited_size * sizeof(bool));
    memset(visited, 0, visited_size * sizeof(bool));

    /* Base case: m = 1 has antisigma 0. */
    visited[0] = true;

    build_spf(LIMIT);

    for (idx_t m = 2; m <= LIMIT; ++m) {
        seq_t sigma = sigma_of_m(m);
        seq_t total = ((seq_t)m) * ((seq_t)m + 1) / 2;     /* sum of 1..m   */
        seq_t antisig = total - sigma;                    /* antisigma(m)  */
        idx_t idx = (idx_t)antisig;                       /* safe cast: idx <= visited_size */
        if (idx < visited_size) visited[idx] = true;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialise();
        initialized = true;
    }
    if (n >= visited_size) return (seq_t)0;
    return visited[n] ? (seq_t)1 : (seq_t)0;
}
