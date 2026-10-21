/*
 * A238952  The size (the number of arcs) in the transitive closure of divisor
 *          lattice D(n).
 * https://oeis.org/A238952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d<n} d(d)
 * Range:     u8 n <= 1079, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (equivalent to the OEIS formula a(n) = Σ_{i=1..⌊n/2⌋} d(i)
 *       * (⌊n/i⌋-⌊(n-1)/i⌋)) */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of divisor counts d(k) for 1 ≤ k ≤ max_cached. */
static idx_t *divcnt = NULL;      /* divcnt[k] = d(k) */
static idx_t  max_cached = 0;     /* largest k for which divcnt is valid */

/* Recompute divisor counts up to 'needed' (inclusive). */
static void recompute_divcnt(idx_t needed)
{
    idx_t size = needed + 1;                 /* array size for indices 0..needed */
    divcnt = (idx_t *)realloc(divcnt, size * sizeof(idx_t));
    if (!divcnt) {                            /* allocation failure – abort */
        abort();
    }
    /* initialise to zero */
    for (idx_t i = 0; i <= needed; ++i) {
        divcnt[i] = 0;
    }
    /* count divisors for each number up to 'needed' */
    for (idx_t i = 1; i <= needed; ++i) {
        for (idx_t j = i; j <= needed; j += i) {
            divcnt[j] += 1;
        }
    }
    max_cached = needed;
}

/* Ensure divisor counts are available up to 'needed' (inclusive). */
static void ensure_divcnt(idx_t needed)
{
    if (needed > max_cached) {
        recompute_divcnt(needed);
    }
}

/* OEIS sequence A238952. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;      /* a(1) = 0 */

    /* divisor counts up to n are needed */
    ensure_divcnt(n);

    seq_t result = 0;
    /* sum d(d) for all proper divisors d of n */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d == 0) {
            result += (seq_t)divcnt[d];
        }
    }
    return result;
}
