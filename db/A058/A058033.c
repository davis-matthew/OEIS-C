/*
 * A058033  Number of powers of 2 between (but not including) two consecutive
 *          primorials.
 * https://oeis.org/A058033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊log₂(Pₙ₊₁)⌋ – ⌊log₂(Pₙ)⌋ , where Pᵢ is the ith primorial
 *            (product of the first i primes). The implementation keeps a
 *            running sum of log₂ of the primes and takes the integer floors
 *            with a small epsilon to guard against floating‑point round‑off
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <math.h>     /* log, log2, floorl */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

static long double *sum_log      = NULL; /* sum_log[i] = log₂(Pᵢ), i ≥ 0 */
static size_t      sum_log_len   = 0;     /* total allocated elements   */
static size_t      sum_log_count = 0;     /* number of valid elements   */

/* Ensure that sum_log contains values up to index `upto` (inclusive). */
static void
ensure_sum_log(idx_t upto)
{
    if (sum_log_len == 0) {
        /* initial allocation: 4 entries are enough for the first few calls */
        sum_log_len = 4;
        sum_log     = (long double *)malloc(sum_log_len * sizeof(long double));
        if (!sum_log) abort();
        sum_log[0] = 0.0L;          /* log₂(P₀) = 0   */
        sum_log_count = 1;          /* only index 0 computed */
    }

    while (sum_log_count <= (size_t)upto) {
        /* New prime to add: primes are 1‑indexed in get_prime (P₁ = 2) */
        uint32_t prime = get_prime((idx_t)sum_log_count);
        long double add = log((long double)prime) / log(2.0L);
        long double new_sum = sum_log[sum_log_count - 1] + add;

        /* Grow storage if necessary */
        if (sum_log_count == sum_log_len) {
            size_t new_len = sum_log_len * 2;
            long double *tmp = (long double *)realloc(sum_log, new_len * sizeof(long double));
            if (!tmp) abort();
            sum_log     = tmp;
            sum_log_len = new_len;
        }

        sum_log[sum_log_count] = new_sum;
        ++sum_log_count;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* below offset: undefined */

    /* We need the cumulative logs up to n+1 */
    ensure_sum_log(n + 1);

    /* Small epsilon to avoid rounding‑down on values very close to an integer. */
    const long double eps = 1e-12L;

    idx_t lo = (idx_t)floorl(sum_log[n]     + eps);
    idx_t hi = (idx_t)floorl(sum_log[n + 1] + eps);

    return (seq_t)(hi - lo);
}
