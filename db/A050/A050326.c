/*
 * A050326  Number of factorizations of n into distinct squarefree numbers >
 *          1.
 * https://oeis.org/A050326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Count the number of ways to write n as a product of distinct squarefree
   integers greater than 1.  The factors are chosen in strictly increasing
   order, so each unordered factorisation is counted exactly once.
   The recursion examines the squarefree divisors of the current remainder;
   only those ≥ the previous factor are allowed, ensuring distinctness
   and preventing permutations.  The base case rem == 1 corresponds to a
   complete factorisation.  For n=1 the empty factorisation is counted as 1. */

#include <stdint.h>
#include <stddef.h>

/* Simple insertion sort for the handful of candidates we generate. */
static void insertion_sort(uint64_t *a, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j-1] > key) {
            a[j] = a[j-1];
            --j;
        }
        a[j] = key;
    }
}

/* Generate all square‑free divisors of `rem` that are ≥ `min_factor`.
   The divisors are returned in ascending order.  The maximum number of
   distinct primes in `rem` is at most 5 (2·3·5·7·11·13 > 10000), so
   the list fits comfortably in a small fixed array. */
static void get_sf_divisors(uint64_t rem, uint64_t min_factor,
                            uint64_t *out, size_t *cnt)
{
    uint64_t primes[6];
    size_t pc = 0;
    uint64_t t = rem;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            primes[pc++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) primes[pc++] = t;       /* last prime factor */

    uint64_t subset = 1ULL << pc;
    size_t pos = 0;

    for (uint64_t mask = 1; mask < subset; ++mask) {
        uint64_t d = 1;
        for (size_t i = 0; i < pc; ++i)
            if (mask & (1ULL << i)) d *= primes[i];

        if (d >= min_factor) out[pos++] = d;
    }

    *cnt = pos;
    insertion_sort(out, pos);
}

/* Recursive count of factorizations. */
static seq_t count_sf(uint64_t rem, uint64_t min_factor)
{
    if (rem == 1) return 1;          /* completed factorisation */

    uint64_t divisors[32];
    size_t div_cnt;
    get_sf_divisors(rem, min_factor, divisors, &div_cnt);
    if (div_cnt == 0) return 0;

    seq_t total = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        total += count_sf(rem / divisors[i], divisors[i] + 1);
    }
    return total;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    if (n == 1) return 1;            /* a(1) = 1 by definition */
    return count_sf((uint64_t)n, 2);  /* smallest allowed factor is 2 */
}
