/*
 * A066637  Total number of elements in all factorizations of n with all
 *          factors > 1.
 * https://oeis.org/A066637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the sum, over all factorizations of n with * factors
 *            > 1, of the number of factors in each factorization. *
 *            Recursively built by iterating over non‑decreasing first factors
 * Range:     u8 n <= 575, u16 n <= 8769, u32 n <= 8769, u64 n <= 8769,
 *            u128 n <= 8769, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Result of a recursive call: total length of factorizations and count of
 * factorizations for a given (n, min_factor). */
typedef struct {
    seq_t sum;    /* sum of lengths of all factorizations */
    seq_t count;  /* number of factorizations */
} Res;

/* Return the pair (sum, count) for n with first factor ≥ min_f. */
static Res dfs(uint64_t n, uint64_t min_f) {
    Res res = {0, 0};

    /* The single‑factor factorization (n itself). */
    res.count = 1;
    res.sum   = 1;

    /* Try each admissible first factor d (non‑decreasing order). */
    for (uint64_t d = min_f; d <= n / d; ++d) {   /* n/d prevents overflow */
        if (n % d != 0) continue;
        Res sub = dfs(n / d, d);
        res.count += sub.count;
        res.sum   += sub.sum + sub.count; /* add one factor for d */
    }
    return res;
}

/* OEIS entry: a(1) = 0, a(n) computed by dfs(n, 2). */
seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* a(1) = 0, small n treated as 0 */
    return dfs((uint64_t)n, 2).sum;
}
