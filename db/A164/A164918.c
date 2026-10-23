/*
 * A164918  The smallest starting prime which reaches prime(n) by repeated
 *          application of the map x->A060308(x).
 * https://oeis.org/A164918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest starting prime which reaches the n‑th prime
 *            * by iterating the map f(x) = largest prime < 2*x. * This
 *            implementation precomputes all prime numbers up to a fixed limit
 *            * (300000) and builds the reverse graph of the map. For each
 *            prime q, * the predecessors p satisfy * q < 2p ≤...
 * Range:     u8 n <= 55, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

static idx_t *prime_vals = NULL;      /* list of primes (ascending)      */
static idx_t  prime_cnt  = 0;          /* number of primes in prime_vals  */
static idx_t *min_start  = NULL;      /* cached minimal start for each prime */

/* ------------------------------------------------------------- */
/* Binary search helpers                                            */
static idx_t lower_index(idx_t val)
/* first index i such that prime_vals[i] >= val */
{
    idx_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prime_vals[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

static idx_t upper_index(idx_t val)
/* last index i such that prime_vals[i] <= val
 * returns prime_cnt if no such index exists                                */
{
    idx_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prime_vals[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0) return prime_cnt;
    return lo - 1;
}

/* ------------------------------------------------------------- */
/* Recursive memoised computation of minimal starting prime     */
static idx_t compute_min_start(idx_t idx)
{
    if (min_start[idx] != 0) return min_start[idx];

    idx_t q      = prime_vals[idx];
    idx_t next_q = (idx + 1 < prime_cnt) ? prime_vals[idx + 1] : q + 1;
    idx_t lower  = (q >> 1) + 1;          /* floor(q/2)+1   */
    idx_t upper  = next_q >> 1;           /* floor(next_q/2) */

    idx_t best = q;                       /* starting from q itself */

    idx_t lo = lower_index(lower);
    idx_t hi = upper_index(upper);
    if (lo < prime_cnt && hi < prime_cnt && lo <= hi) {
        for (idx_t i = lo; i <= hi; ++i) {
            idx_t cand = compute_min_start(i);
            if (cand < best) best = cand;
        }
    }
    min_start[idx] = best;
    return best;
}

/* ------------------------------------------------------------- */
/* Lazily initialise primes and memoisation table               */
static void init_once(void)
{
    static bool done = false;
    if (done) return;

    /* 300 000 is enough for the first 10 000 primes (≈ 104 729). */
    const idx_t LIMIT = 300000;
    idx_t *primes_raw = NULL;
    size_t cnt_raw    = 0;

    build_primes(LIMIT, &primes_raw, &cnt_raw);

    prime_cnt = (idx_t)cnt_raw;
    prime_vals = (idx_t *)malloc((size_t)prime_cnt * sizeof(idx_t));
    for (idx_t i = 0; i < prime_cnt; ++i) prime_vals[i] = primes_raw[i];
    free(primes_raw);

    min_start = (idx_t *)calloc((size_t)prime_cnt, sizeof(idx_t));

    done = true;
}

/* ------------------------------------------------------------- */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unsupported offset, guard */

    /* initialise once */
    init_once();

    idx_t idx = n - 1;                /* convert to 0‑based index   */
    if (idx >= prime_cnt) return 0;   /* out of range – not expected */

    idx_t result_idx = compute_min_start(idx);
    return (seq_t)result_idx;        /* cast to seq_t (value fits) */
}
