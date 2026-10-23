/*
 * A106962  Primes of the form x^2+xy+20y^2, with x and y nonnegative.
 * https://oeis.org/A106962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form x^2 + x*y + 20*y^2, with x,y ≥ 0 The function
 *            generates all such primes up to a growing bound and keeps them
 *            sorted to answer a(n) in O(1)
 * Range:     u8 n <= 2, u16 n <= 592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq, int_sqrt, is_prime, push_seq, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/push_seq.h"
#include "../utils/cmp_seq.h"
#include "../utils/int_sqrt.h"
#include "../utils/seq_malloc.h"

/* Global storage for primes */
static seq_t *primes = NULL;
static idx_t primes_cap = 0;
static idx_t primes_size = 0;

/* Highest value that has been examined so far */
static idx_t max_checked_value = 0;

/* Forward declarations */
static void generate_up_to(idx_t bound);
static void ensure_n(idx_t n);

/* Generate primes of the form f(x,y)=x^2+xy+20y^2
   whose values are in (max_checked_value, bound] */
static void generate_up_to(idx_t bound)
{
    if (bound <= max_checked_value) return;

    idx_t y_limit = int_sqrt(bound / 20ULL);  /* y^2 * 20 <= bound */
    for (idx_t y = 0; y <= y_limit; ++y) {
        uint64_t y2 = (uint64_t)y * (uint64_t)y;
        for (idx_t x = 0; ; ++x) {
            /* f = x^2 + x*y + 20*y^2 */
            uint64_t val = (uint64_t)x * (uint64_t)x
                         + (uint64_t)x * (uint64_t)y
                         + 20ULL * y2;
            if (val > (uint64_t)bound) break;
            if (val <= (uint64_t)max_checked_value) continue;
            if (val >= 2ULL && is_prime((uint64_t)val)) {
                push_seq(&primes, &primes_cap, &primes_size, (seq_t)val);
            }
        }
    }

    /* Sort and deduplicate the prime list */
    if (primes_size > 0) {
        qsort(primes, (size_t)primes_size, sizeof(seq_t), cmp_seq);

        /* Compact in place: keep unique values only */
        idx_t uniq = 0;
        for (idx_t i = 0; i < primes_size; ++i) {
            if (i == 0 || cmp_seq(&primes[i], &primes[i - 1]) != 0) {
                primes[uniq++] = primes[i];
            }
        }
        primes_size = uniq;
    }

    max_checked_value = bound;
}

/* Ensure that we have computed at least n primes. */
static void ensure_n(idx_t n)
{
    if (primes_size >= n) return;

    /* Start with a modest bound and double it until enough primes are found. */
    idx_t bound = 100ULL;
    while (primes_size < n) {
        generate_up_to(bound);
        if (primes_size >= n) break;
        /* Avoid overflow – use a 128‑bit temporary? For n up to 10000 the bound stay below 2^63. */
        bound = bound << 1;   /* multiply by 2 */
    }
}

/* Main OEIS sequence function: a(n) = n-th prime of the form x^2 + xy + 20y^2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; 0 is dummy */
    ensure_n(n);
    return primes[(size_t)n - 1];
}
