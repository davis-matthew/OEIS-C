/*
 * A064542  a(n) = Max { k | k! <= n! / k! } where m! = A000142(m), factorial.
 * https://oeis.org/A064542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 455, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A064542 – a(n) = max{ k | k!^2 ≤ n! }
    Computation is done by comparing logarithms of factorials:
        2*log(k!) ≤ log(n!)
    The log factorials are pre‑computed in a double array.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -------------- log factorial table ------------------------------- */
static size_t log_fact_size = 0;        /* number of entries stored (0…size-1)   */
static double *log_fact = NULL;        /* log_fact[i] = ln(i!)                   */

static void ensure_log_fact(size_t n)
{
    if (n < log_fact_size) return;

    size_t new_size = n + 1;
    double *new_array = (double *)realloc(log_fact, new_size * sizeof(double));
    if (!new_array) exit(EXIT_FAILURE);
    log_fact = new_array;

    if (log_fact_size == 0) log_fact[0] = 0.0;
    for (size_t i = log_fact_size; i <= n; ++i)
        log_fact[i] = (i == 0 ? 0.0 : log_fact[i-1] + log((double)i));

    log_fact_size = new_size;
}

/* -------------- memoization of a(n) ------------------------------ */
static seq_t *cache = NULL;
static size_t cache_size = 0;

static void ensure_cache(size_t n)
{
    if (n < cache_size) return;
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    for (size_t i = cache_size; i <= n; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* ------------------------ sequence ------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined for n=0, keep harmless   */

    size_t idx = (size_t)n;

    ensure_cache(idx);
    if (cache[idx]) return cache[idx];

    /* build log‑factorial table up to n */
    ensure_log_fact(idx);

    /* binary search for the largest k with 2*log(k!) ≤ log(n!) */
    size_t low = 1, high = idx, best = 1;
    const double eps = 1e-12;              /* protects against rounding errors   */
    double logn = log_fact[idx];

    while (low <= high) {
        size_t mid = (low + high) >> 1;
        double val = 2.0 * log_fact[mid];
        if (val <= logn + eps) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cache[idx] = (seq_t)best;
    return cache[idx];
}
