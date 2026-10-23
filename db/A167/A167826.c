/*
 * A167826  a(n) is the number of n-tosses having a run of 3 or more heads and
 *          a run of 3 or more tails for a fair coin.
 * https://oeis.org/A167826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2*(tribonacci(n+3) - Fibonacci(n+1))
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/fib.h"

static seq_t *trib_cache = NULL;
static idx_t   trib_max   = 0;

/* Ensure trib_cache[0..k] are initialized */
static void ensure_trib(idx_t k)
{
    if (k <= trib_max)
        return;

    idx_t new_size = k + 1;
    seq_t *tmp = NULL;

    if (trib_cache == NULL) {
        trib_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(trib_cache, new_size * sizeof(seq_t));
        trib_cache = tmp;
    }

    /* Initialize from trib_max + 1 up to k */
    for (idx_t i = trib_max + 1; i <= k; ++i) {
        if (i == 0) {
            trib_cache[i] = 0;
        } else if (i == 1) {
            trib_cache[i] = 0;
        } else if (i == 2) {
            trib_cache[i] = 1;
        } else {
            trib_cache[i] = trib_cache[i - 1] + trib_cache[i - 2] + trib_cache[i - 3];
        }
    }
    trib_max = k;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 we return 0. */
    if (n == 0)
        return 0;

    /* Ensure we have the required Tribonacci value */
    ensure_trib(n + 3);

    /* Compute 2^n */
    seq_t pow2n = pow2(n);

    /* Compute the difference tribonacci(n+3) - Fibonacci(n+1) */
    seq_t diff = trib_cache[n + 3] - fib(n + 1);

    /* a(n) = 2^n - 2 * diff */
    seq_t result = pow2n - (diff * 2);

    return result;
}
