/*
 * A056127  Minimum m where product_{k=1 to m}[p_k] > (p_{m+1})^n, where p_k
 *          is k-th prime.
 * https://oeis.org/A056127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum m where product_{k=1..m} p_k > p_{m+1}^n  (p_k = k‑th prime) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime.h"

/* ---------- static state for primes and cumulative logs ---------- */

static uint64_t *primes   = NULL;   /* primes[0] = 2, etc.                */
static size_t   primes_cnt = 0;     /* number of primes currently stored  */
static size_t   primes_cap = 0;     /* allocated capacity of the primes   */

static double *cum_log   = NULL;    /* cum_log[i] = Σ_{k=0}^{i-1} log(p_k) */
static size_t  log_cnt    = 0;      /* number of log entries that are valid */
static size_t  log_cap    = 0;      /* allocated capacity of the log array */

/* Ensure that at least 'needed' primes (indexed from 0) are available.
   Also compute cumulative logs up to that many primes. */
static void ensure_primes_needed(size_t needed)
{
    if (primes_cnt >= needed) return;

    /* Grow the primes array if necessary */
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Generate missing primes starting after the last stored prime */
    uint64_t start = (primes_cnt == 0) ? 2ULL : primes[primes_cnt - 1] + 1ULL;
    for (uint64_t v = start; primes_cnt < needed; ++v) {
        if (is_prime(v)) primes[primes_cnt++] = v;
    }

    /* Grow the cumulative‑log array if necessary */
    if (log_cap < primes_cnt + 1) {
        size_t new_cap = log_cap ? log_cap * 2 : 16;
        if (new_cap < primes_cnt + 1) new_cap = primes_cnt + 1;
        double *tmp = (double*)realloc(cum_log, new_cap * sizeof(double));
        if (!tmp) exit(EXIT_FAILURE);
        cum_log = tmp;
        log_cap = new_cap;
    }

    /* Initialise the log array */
    if (log_cnt == 0) {
        cum_log[0] = 0.0;
        log_cnt = 1;
    }
    for (size_t i = log_cnt - 1; i < primes_cnt; ++i) {
        cum_log[i + 1] = cum_log[i] + log((double)primes[i]);
    }
    log_cnt = primes_cnt + 1;
}

/* ---------- OEIS sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    for (idx_t m = 1; ; ++m) {
        /* Need primes up to index m (p_1 … p_m) and the next prime p_{m+1}. */
        ensure_primes_needed((size_t)m + 1);

        /* Logarithm of the product of the first m primes. */
        double left  = cum_log[(size_t)m];
        /* n * log(p_{m+1}). p_{m+1} is primes[m] because indices are 0‑based. */
        double right = (double)n * log((double)primes[(size_t)m]);

        if (left > right) return (seq_t)m;
    }
}
