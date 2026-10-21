/*
 * A054725  a(1)=1; a(n) = Sum_{p | n} e * a(p-1), where sum is over all
 *          primes p that divide n, and e is the multiplicity of p in n.
 * https://oeis.org/A054725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e || n} e * a(p-1) with a(1) = 1. For every n ≥ 2,
 *            the sum is taken over all distinct prime divisors p of n, e
 *            being the exponent of p in n. The function memoises results in a
 *            static table so that calls in any order are efficient
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;          /* computed values */
static bool *a_computed = NULL;        /* flags for computed entries */
static size_t a_cache_size = 0;        /* allocated size */

/* Ensure the cache can hold at least indices up to cap-1. */
static void ensure_a_cache_size(size_t cap)
{
    if (cap <= a_cache_size)
        return;
    size_t new_cap = cap > 1 ? cap*2 : 16;
    if (new_cap < cap) new_cap = cap;          /* avoid overflow */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_computed = (bool *)realloc(a_computed, new_cap * sizeof(bool));
    /* initialise new part */
    for (size_t i = a_cache_size; i < new_cap; ++i) {
        a_computed[i] = false;
    }
    a_cache_size = new_cap;
    if (!a_cache || !a_computed) {
        /* allocation failed – abort so that subsequent uses fail cleanly */
        abort();
    }
}

/* Compute a(n) recursively with memoisation */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)          /* base case */
        return (seq_t)1;

    /* Ensure cache can hold n */
    ensure_a_cache_size((size_t)n + 1);

    if (a_computed[n])
        return a_cache[n];

    seq_t sum = (seq_t)0;
    idx_t m = n;

    /* trial division factorisation */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* p is prime divisor */
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            sum += (seq_t)e * a((idx_t)(p - 1));
        }
    }

    if (m > 1) { /* remaining prime factor */
        sum += (seq_t)1 * a((idx_t)(m - 1));
    }

    a_cache[n] = sum;
    a_computed[n] = true;
    return sum;
}
