/*
 * A025081  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Fibonacci numbers), t = (composite numbers).
 * https://oeis.org/A025081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌈n/2⌉} s(i) * t(n-i+1), where s(i) =
 *            Fibonacci(i) and t(j) = the j‑th composite number >5. (Composite
 *            numbers listed as 6, 8, 9, 10, 12, …)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Fibonacci cache (0‑based; fibs[0] = F(1) = 1) */
static seq_t *fibs      = NULL;
static size_t fibs_cap  = 0;
static size_t fibs_len  = 0;   /* number of computed Fibonacci numbers */

/* Composite cache (0‑based; comps[0] = 6) */
static seq_t *comps      = NULL;
static size_t comps_cap  = 0;
static size_t comps_len  = 0;  /* number of computed composites */
static uint64_t comps_next_val = 6;   /* next integer to test for compositeness */

/* Grow a seq_t array to at least 'needed' elements. */
static void *grow_array(void *ptr, size_t *cap, size_t needed)
{
    size_t sz = (*cap > 0) ? (*cap * 2) : 16;
    while (sz < needed) sz <<= 1;              /* double until enough */
    void *newp = realloc(ptr, sz * sizeof(seq_t));
    if (!newp) exit(1);                       /* allocation failed */
    *cap = sz;
    return newp;
}

/* Ensure at least 'target' Fibonacci numbers are available. */
static void ensure_fibs(size_t target)
{
    if (target <= fibs_len) return;

    /* Ensure capacity. */
    if (fibs_cap < target)
        fibs = (seq_t *)grow_array(fibs, &fibs_cap, target);

    /* Compute new Fibonacci numbers. */
    for (size_t i = fibs_len; i < target; ++i) {
        if (i < 2)          /* F(1) = F(2) = 1 */
            fibs[i] = 1;
        else
            fibs[i] = fibs[i - 1] + fibs[i - 2];
    }
    fibs_len = target;
}

/* Ensure at least 'needed' composite numbers (>5) are available. */
static void ensure_comps(size_t needed)
{
    while (comps_len < needed) {
        /* Grow array when full. */
        if (comps_cap == comps_len)
            comps = (seq_t *)grow_array(comps, &comps_cap, comps_len + 1);

        /* Test next integer for compositeness. */
        if (!is_prime(comps_next_val)) {
            comps[comps_len++] = (seq_t)comps_next_val;
        }
        ++comps_next_val;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1; safety */

    idx_t k = (n + 1) / 2;              /* ceil(n/2) with unsigned arithmetic */

    /* Ensure caches contain needed elements. */
    ensure_fibs((size_t)k);
    ensure_comps((size_t)n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        size_t fi = (size_t)(i - 1);          /* Fibonacci index (0‑based) */
        size_t cj = (size_t)(n - i);          /* Composite index (0‑based) */
        sum += fibs[fi] * comps[cj];
    }
    return sum;
}
