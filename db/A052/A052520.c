/*
 * A052520  Number of pairs of sequences of cardinality at least 2.
 * https://oeis.org/A052520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-3) * n! for n > 2 a(0) = a(1) = a(2) = a(3) = 0
 *            Sequence A052520: number of pairs of sequences of cardinality at
 *            least 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL; /* factorial cache: fact_cache[i] = i! */
static idx_t fact_size = 0;      /* number of factorial entries currently stored (size of valid part) */
static idx_t fact_cap  = 0;      /* current allocation capacity */

/* Ensure that fact_cache contains factorials up to index n (inclusive). */
static void ensure_factorial(idx_t n)
{
    /* grow allocation as needed (doubling strategy) */
    if (fact_size <= n) {
        idx_t needed = n + 1;
        if (fact_cap < needed) {
            idx_t new_cap = fact_cap ? fact_cap << 1 : 1;
            while (new_cap < needed) new_cap <<= 1;
            fact_cache = (seq_t *)realloc(fact_cache, (size_t)new_cap * sizeof(seq_t));
            fact_cap = new_cap;
        }

        /* compute missing factorials */
        for (idx_t i = fact_size; i <= n; ++i) {
            if (i == 0) {
                fact_cache[0] = (seq_t)1;
            } else {
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
            }
        }
        fact_size = needed;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    ensure_factorial(n);
    return (seq_t)(n - 3) * fact_cache[n];
}
