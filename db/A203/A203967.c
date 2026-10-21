/*
 * A203967  The number of positive integers <= n that have a prime number of
 *          divisors.
 * https://oeis.org/A203967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1492, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203967: a(n) = #{ m ≤ n | d(m) is prime }
   where d(m) is the number of positive divisors of m. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static cache: divisor counts (1‑based) and running prefix sums. */
static unsigned short *div_arr   = NULL;   /* divisor counts */
static size_t          div_cap  = 0;        /* capacity of div_arr */

static seq_t *prefix_arr = NULL;           /* prefix sums (1‑based) */
static size_t prefix_cap  = 0;              /* capacity of prefix_arr */

static idx_t max_computed = 0;              /* largest n that has been computed */

/* Helper: test whether a small unsigned integer is prime. */
static bool is_small_prime(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Main sequence function.  The function may be called in any order; it
   caches computed values up to the maximum n seen so far. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* out of bounds */

    if (n > max_computed) {                     /* need to extend the caches */
        size_t new_cap = (size_t)n + 1;          /* +1 for 1‑based indexing */

        /* Resize divisor array if necessary. */
        if (new_cap > div_cap) {
            unsigned short *tmp = (unsigned short *)
                realloc(div_arr, new_cap * sizeof(unsigned short));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            div_arr = tmp;
            div_cap = new_cap;
        }

        /* Resize prefix array if necessary. */
        if (new_cap > prefix_cap) {
            seq_t *tmp2 = (seq_t *)
                realloc(prefix_arr, new_cap * sizeof(seq_t));
            if (!tmp2) return (seq_t)0;          /* allocation failed */
            prefix_arr = tmp2;
            prefix_cap = new_cap;
        }

        /* Zero the entire divisor array. */
        memset(div_arr, 0, new_cap * sizeof(unsigned short));

        /* Sieve to compute divisor counts. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++div_arr[j];
            }
        }

        /* Compute prefix sums of numbers with prime divisor counts. */
        prefix_arr[0] = (seq_t)0;
        for (idx_t i = 1; i <= n; ++i) {
            prefix_arr[i] = prefix_arr[i - 1] +
                            (seq_t)(is_small_prime(div_arr[i]) ? 1 : 0);
        }

        max_computed = n;
    }

    return prefix_arr[n];
}
