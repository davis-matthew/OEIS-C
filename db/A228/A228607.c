/*
 * A228607  a(n) is the number of independent vertex subsets (i.e., the
 *          Merrifield-Simmons index) of the triangulane T[n] defined in the
 *          Khalifeh et al. and Deutsch et al. references.
 * https://oeis.org/A228607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n)^3 + 3*c(n)*d(n)^2 Recurrence: c(1) = 1, d(1) = 3
 *            c(n) = d(n-1)^2 d(n) = 2*c(n-1)*d(n-1) + d(n-1)^2 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: values are memoized in static arrays that grow as needed.
 * The helper seq_realloc is available from the build utilities.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* static caches for c[ ] and d[ ] values */
static seq_t *c_cache = NULL;
static seq_t *d_cache = NULL;
static size_t cache_cap = 0;   /* capacity of the allocated arrays (>= required size) */
static idx_t cache_max = 0;    /* highest n that has been computed (>=1) */

/* ensure the caches contain values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* arrays will store indices 1..n inclusive; index 0 remains unused */
    size_t new_cap = (size_t)(n + 1);
    if (cache_cap < new_cap) {
        seq_t *new_c = seq_realloc(c_cache, cache_cap, new_cap);
        seq_t *new_d = seq_realloc(d_cache, cache_cap, new_cap);
        c_cache = new_c;
        d_cache = new_d;
        cache_cap = new_cap;
    }

    if (cache_max == 0) {
        /* initialise the first entry when the caches are empty */
        c_cache[1] = 1;
        d_cache[1] = 3;
        cache_max = 1;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t di_minus1 = d_cache[i - 1];
        seq_t ci_minus1 = c_cache[i - 1];
        seq_t d_sq = di_minus1 * di_minus1;
        c_cache[i] = d_sq;
        d_cache[i] = 2 * ci_minus1 * di_minus1 + d_sq;
    }

    cache_max = n;
}

/* compute the n-th term of the sequence */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but return 0 */

    ensure_cache(n);

    seq_t dn = d_cache[n];
    seq_t cn = c_cache[n];
    seq_t dn_sq = dn * dn;
    seq_t part1 = dn_sq * dn;      /* d(n)^3 */
    seq_t part2 = 3 * cn * dn_sq;  /* 3*c(n)*d(n)^2 */
    return part1 + part2;
}
