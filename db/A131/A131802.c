/*
 * A131802  Sequence related to factorizations and prime signatures: a(1) = 1;
 *          for n>1, a(n) = A057567(n) - 2*A001055(n).
 * https://oeis.org/A131802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131802
 *
 * a(1) = 1
 * for n > 1 :  a(n) = sum_{d | n} A001055(d) - 2*A001055(n)
 *
 * A001055(n) = number of unordered multiplicative partitions of n
 *              (count of ways to factor n as a product of integers > 1,
 *              unordered and disregarding order, including n itself).
 *
 * The implementation uses a simple recursive definition for A001055.
 * Results are memoised in a dynamic array to avoid repeated work.
 * The function a(idx_t n) has external linkage as requested.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ensure_cache helper is not used; a custom dynamic cache is implemented below. */

/* dynamic array for memoisation of A001055 values */
static seq_t *A001055_cache = NULL;   /* one‑based array: A001055_cache[i] = A001055(i) */
static size_t A001055_cache_cap = 0;  /* current capacity of the array */

/* enlarge the cache so that indices up to 'n' are valid */
static void
ensure_A001055_cache(idx_t n)
{
    if (n + 1 <= A001055_cache_cap)
        return;
    size_t new_cap = A001055_cache_cap ? A001055_cache_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;                     /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(A001055_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort execution (undefined behaviour in OEIS eval) */
        abort();
    }
    /* initialise new cells to 0 (unused) */
    for (size_t i = A001055_cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    A001055_cache = tmp;
    A001055_cache_cap = new_cap;
}

/* recursive routine to count unordered multiplicative partitions of n,
 * starting with factors >= min_factor.  The function is only invoked
 * with min_factor >= 2. */
static seq_t
count_partitions(idx_t n, idx_t min_factor)
{
    /* trivial partition: the number itself */
    seq_t total = 1;

    for (idx_t f = min_factor; f * f <= n; ++f) {
        if (n % f == 0) {
            total += count_partitions(n / f, f);
        }
    }
    return total;
}

/* wrapper that returns A001055(n) using memoisation */
static seq_t
A001055_value(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_A001055_cache(n);
    if (A001055_cache[n] != 0 || n == 1)
        return A001055_cache[n] ? A001055_cache[n] : (A001055_cache[n] = 1);

    seq_t val = count_partitions(n, 2);
    A001055_cache[n] = val;
    return val;
}

/* OEIS sequence A131802 */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;   /* by definition */

    seq_t A_n = A001055_value(n);
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += A001055_value(d);
            if (d * d != n) {
                sum += A001055_value(n / d);
            }
        }
    }

    return sum - (seq_t)2 * A_n;
}
