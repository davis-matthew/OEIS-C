/*
 * A175839  Smallest runs of n*2-1 consecutive composites.
 * https://oeis.org/A175839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A175839 – Smallest runs of n*2-1 consecutive composites.
 *
 *  For a given n ≥ 1 let r = ⌈√n⌉.  All composites in the
 *  *r‑th* run are consecutive and there are exactly 2r‑1 of them.
 *  The (r‑1)²‑th index is the beginning of run r−1, so the current
 *  index n lies r²−(r−1)² = 2r−1 positions into run r.
 *
 *  Let `run_start[r]` be the smallest integer from which
 *  a run of length 2r‑1 starts.  Then
 *
 *      a(n) = run_start[r] + (n - (r-1)² - 1).
 *
 *  All run_start values are computed on demand by searching forward
 *  from the previous run start; no generated run is ever smaller
 *  than the previous one's start.  An Eratosthenes sieve is used
 *  for composites up to a fixed upper bound, and larger
 *  numbers are checked by the provided `is_prime_big()` routine.
 *  The function is memoised and works for any of the supported
 *  integer types (`seq_t`).  It will handle any call order,
 *  and is efficient enough for n up to ~10 000 (r ≤ 100).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

/* ----------  small sieve for quick primality test  ---------- */
static size_t sieve_limit = 0;
static uint8_t *sieve = NULL;

static void init_sieve(void)
{
    if (sieve_limit != 0) return;   /* already initialised */

    /* limit comfortably above the 10 000th run (r = 100) */
    sieve_limit = 10000000;
    sieve = (uint8_t *)malloc((sieve_limit + 1) * sizeof(uint8_t));
    if (!sieve) abort();

    for (size_t i = 0; i <= sieve_limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t p = 2; p * p <= sieve_limit; ++p) {
        if (sieve[p]) {
            for (size_t q = p * p; q <= sieve_limit; q += p) sieve[q] = 0;
        }
    }
}

static bool is_composite(seq_t n)
{
    if ((size_t)n <= sieve_limit) return sieve[(size_t)n] == 0;
    return !is_prime_big(n);
}

/* ----------  data for run starts  ---------- */
#define MAX_RUN 200   /* enough for n ≤ 10 000  (r ≤ 100) */
static seq_t run_start[MAX_RUN + 1];
static bool run_start_computed[MAX_RUN + 1] = {false};
static seq_t prev_run_start = 0;   /* start of the last computed run (r-1) */

/* Compute run_start[r] (length 2r‑1), using prev_run_start as a lower bound. */
static void compute_run(idx_t r)
{
    if (run_start_computed[r]) return;           /* already known */

    if (r == 1) {                                /* run 1 */
        run_start[1] = 4;
        prev_run_start = run_start[1];
        run_start_computed[1] = true;
        return;
    }

    seq_t candidate = prev_run_start;   /* a run may start at the same place as the previous one */
    idx_t length = 2 * r - 1;

    while (true) {
        bool ok = true;
        for (idx_t off = 0; off < length; ++off) {
            seq_t val = candidate + (seq_t)off;
            if (!is_composite(val)) { ok = false; break; }
        }
        if (ok) {
            if (r > MAX_RUN) abort();   /* safety check */
            run_start[r] = candidate;
            prev_run_start = candidate;
            run_start_computed[r] = true;
            return;
        }
        ++candidate;
    }
}

/* Ensure that all run starts up to r are computed. */
static void ensure_runs(idx_t r)
{
    for (idx_t k = 1; k <= r; ++k)
        if (!run_start_computed[k]) compute_run(k);
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    init_sieve();

    /* find r such that (r-1)^2 < n ≤ r^2  */
    idx_t r = (idx_t)sqrtl((long double)n);
    if (r * r < n) ++r;

    /* offset inside run r (0‑based) */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t offset  = n - prev_sq - 1;   /* 0 ≤ offset < 2r-1 */

    ensure_runs(r);
    return run_start[r] + (seq_t)offset;
}
