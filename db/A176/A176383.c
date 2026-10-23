/*
 * A176383  Triangle of the powers of the prime factorization of n! in row n.
 * https://oeis.org/A176383
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) enumerates the prime powers in n! read row‑wise. For n≥2: k
 *            = n-2 (0‑based linear index). Find r such that cum[r-1] ≤ k <
 *            cum[r], where cum[r] = Σ_{i=2}^r π(i) (π(i) = number of primes ≤
 *            i). Let idx = k - cum[r-1]; then a(n) = p_idx^{e(r,p_idx)} where
 *            e(r,p) = Σ_{t≥1} ⌊r/p^t⌋
 * Range:     u8 n <= 24, u16 n <= 69, u32 n <= 215, u64 n <= 688,
 *            u128 n <= 2311, big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* utilities */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

/* static cache */
static bool          init_done      = false;
static uint64_t     *primes         = NULL;   /* primes up to current limit */
static size_t        prime_cnt      = 0;      /* number of primes */
static idx_t        *cum            = NULL;   /* cumulative term counts */
static idx_t        max_limit       = 0;      /* highest n precomputed */

/* ------------------------------------------------------------------ */
/* Compute exponent of prime p in r!  (Legendre's formula)            */
static idx_t
exponent_in_factorial(idx_t r, uint64_t p)
{
    idx_t e = 0;
    for (idx_t div = r / p; div > 0; div /= p)
        e += div;
    return e;
}

/* ------------------------------------------------------------------ */
/* Ensure that tables are built up to at least n.  Rebuild if needed  */
static void
ensure_precomputed(idx_t n)
{
    if (init_done && n <= max_limit)
        return;

    /* free old tables if any */
    if (cum)
        free(cum);
    if (primes)
        free(primes);

    /* build primes up to n */
    build_primes_upto(n, &primes, &prime_cnt);

    /* allocate cumulative array */
    size_t sz = (size_t)n + 1;
    cum = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!cum) {
        exit(EXIT_FAILURE);  /* allocation failed */
    }

    /* compute cumulative number of terms up to each row r */
    size_t pi_cur   = 0;          /* current value of π(r) */
    size_t pi_index = 0;          /* index into primes array */
    cum[0] = 0;
    for (idx_t r = 1; r <= n; ++r) {
        if (pi_index < prime_cnt && primes[pi_index] == r) {
            ++pi_cur;
            ++pi_index;
        }
        cum[r] = cum[r - 1] + pi_cur;
    }

    max_limit = n;
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t
a(idx_t n)
{
    if (n < 2)
        return seq_pow((seq_t)0, 0);   /* undefined for n<2 */

    ensure_precomputed(n);

    /* linear 0‑based index of the term in the flattened triangle */
    idx_t k = n - 2;

    /* binary search for the row r: cum[r-1] ≤ k < cum[r] */
    idx_t low = 1, high = max_limit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum[mid] > k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row containing the term */

    idx_t idx_in_row = k - cum[r - 1]; /* 0‑based position inside the row */
    uint64_t prime = primes[idx_in_row];

    idx_t exp = exponent_in_factorial(r, prime);

    return seq_pow((seq_t)prime, exp);
}
