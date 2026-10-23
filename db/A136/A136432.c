/*
 * A136432  a(n)! is the smallest factorial bigger than n^n.
 * https://oeis.org/A136432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that k! > n^n (OEIS A136432)
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Logarithm of factorial array: log_fact[i] = ln(i!) for i ≥ 0 */
static long double *log_fact = NULL;
static size_t log_cap   = 0;

/* Ensure log_fact has capacity for indices 0 … needed-1.  All new
   entries are initialized incrementally. */
static void
ensure_log_capacity(size_t needed)
{
    if (needed <= log_cap) return;

    size_t new_cap = log_cap ? log_cap : 4;
    while (new_cap < needed) new_cap <<= 1;

    long double *tmp = (long double *)realloc(log_fact, new_cap * sizeof(long double));
    if (!tmp) abort();          /* allocation failure */
    log_fact = tmp;

    /* Initialize from old capacity to new capacity */
    if (log_cap == 0) {
        log_fact[0] = 0.0L;     /* 0! = 1, ln=0 */
        log_fact[1] = 0.0L;     /* 1! = 1, ln=0 */
    }
    for (size_t i = (log_cap > 1 ? log_cap : 2); i < new_cap; ++i) {
        log_fact[i] = log_fact[i - 1] + logl((long double)i);
    }
    log_cap = new_cap;
}

/* Return the smallest k such that k! > n^n.  Returns the value
   as a seq_t. */
seq_t a(idx_t n)
{
    /* Special case n = 0: 0^0 is treated as 1, so 2! = 2 > 1. */
    if (n == 0) return (seq_t)2;

    long double target = (long double)n * logl((long double)n);

    /* Find a range [low, high] with log_fact[high] > target */
    size_t low  = 1;
    size_t high = (size_t)(n + 10);
    ensure_log_capacity(high + 1);
    while (log_fact[high] <= target) {
        high <<= 1;
        ensure_log_capacity(high + 1);
    }

    /* Binary search on log_fact for the first index > target */
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (log_fact[mid] > target)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
