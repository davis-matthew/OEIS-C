/*
 * A060764  Number of divisors of n which are not also differences between
 *          consecutive divisors (ordered by increasing magnitude) of n.
 * https://oeis.org/A060764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not equal to any
 *            difference of consecutive divisors of n (divisors listed in
 *            increasing order)
 * Range:     u8 n <= 9977, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   binary_search_uint64, factor_and_divisor_count, generate_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binary_search_uint64.h"
#include "../utils/factor_and_divisor_count.h"
#include "../utils/generate_divs.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Comparator for qsort on uint64_t arrays. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t av = *(const uint64_t *)a;
    uint64_t bv = *(const uint64_t *)b;
    return (av < bv) ? -1 : (av > bv);
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n=0, return 0 safely */

    uint64_t num = (uint64_t)n;      /* work in native uint64_t */

    /* Factorisation */
    uint64_t pr[32];
    uint64_t exp[32];
    size_t cnt = 0;
    uint64_t tau = factor_and_divisor_count(num, pr, exp, &cnt);

    /* Allocate space for all divisors */
    uint64_t *divs = (uint64_t *)malloc(tau * sizeof(uint64_t));
    if (!divs) return (seq_t)0;       /* allocation failure */

    size_t pos = 0;
    generate_divs(1ULL, 0, divs, &pos, pr, exp, cnt);
    /* pos should equal tau */
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    /* Compute differences between consecutive divisors */
    size_t diff_count = (tau > 0) ? tau - 1 : 0;
    uint64_t *diffs = NULL;
    if (diff_count > 0) {
        diffs = (uint64_t *)malloc(diff_count * sizeof(uint64_t));
        if (!diffs) { free(divs); return (seq_t)0; }
        for (size_t i = 0; i < diff_count; ++i)
            diffs[i] = divs[i + 1] - divs[i];
        qsort(diffs, diff_count, sizeof(uint64_t), cmp_uint64);
    }

    /* Count divisors that are not equal to any difference */
    size_t not_diff = 0;
    for (size_t i = 0; i < tau; ++i) {
        if (!binary_search_uint64(diffs, diff_count, divs[i]))
            ++not_diff;
    }

    /* Clean up */
    free(divs);
    free(diffs);

    return (seq_t)not_diff;
}
