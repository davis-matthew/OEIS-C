/*
 * A050370  Number of ways to factor n into composite factors.
 * https://oeis.org/A050370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to factor n into composite factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation recursively counts unordered factorizations of n using
 *  only composite integers (>1 and not prime).  The only global state is a
 *  sieve of composites up to the largest n requested. */

#include <stdint.h>
#include <stdlib.h>

/* global arrays for composite information */
static uint8_t *is_comp = NULL;   /* 0 = not composite, 1 = composite */
static size_t comp_cap = 0;       /* size of is_comp (>= index+1) */

/* integer square root via binary search */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ensure composite table covers all indices up to limit */
static void ensure_composite(idx_t limit)
{
    if ((size_t)(limit + 1) <= comp_cap) return;

    size_t new_cap = (size_t)(limit + 1);
    uint8_t *prime = (uint8_t *)malloc(new_cap);
    if (!prime) return;                     /* allocation failed – ignore */

    for (size_t i = 0; i < new_cap; ++i) prime[i] = 1;
    if (new_cap > 0) prime[0] = 0;
    if (new_cap > 1) prime[1] = 0;
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (prime[i]) {
            for (size_t j = i * i; j < new_cap; j += i) prime[j] = 0;
        }
    }

    uint8_t *comp = (uint8_t *)malloc(new_cap);
    if (!comp) {
        free(prime);
        return;
    }
    for (size_t i = 0; i < new_cap; ++i) comp[i] = 0;
    for (size_t i = 4; i < new_cap; ++i) if (!prime[i]) comp[i] = 1;

    if (is_comp) free(is_comp);
    free(prime);
    is_comp = comp;
    comp_cap = new_cap;
}

/* recursive count of factorizations of n using composites ≥ min_factor */
static seq_t count_fact(idx_t n, idx_t min_factor)
{
    if (n < min_factor) return 0;

    seq_t count = 0;
    /* count the trivial factorization consisting of n itself, if n is composite */
    if (n < comp_cap && is_comp[n]) ++count;

    idx_t start = min_factor;
    if (start < 4) start = 4;          /* smallest composite */
    idx_t lim = int_sqrt(n);

    for (idx_t f = start; f <= lim; ++f) {
        if (f >= comp_cap || !is_comp[f]) continue;
        if (n % f) continue;
        idx_t g = n / f;
        if (g < f) continue;          /* keep factors non‑decreasing */
        count += count_fact(g, f);
    }
    return count;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;                /* by definition */
    ensure_composite(n);
    return count_fact(n, 4);
}
