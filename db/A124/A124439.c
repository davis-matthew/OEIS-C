/*
 * A124439  Complete list of solutions to y^2 = x^3 + 17; sequence gives x
 *          values.
 * https://oeis.org/A124439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   this implementation computes the complete list of integer
 *            solutions to y² = x³ + 17 (the sequence of x‑values), sorted in
 *            the Mordell ordering: by increasing |x|, negative values before
 *            positive when |x| is equal. The sequence is finite
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   cmp_mordell
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_mordell.h"

/* Upper bound for the search; large enough to contain all known
 * integer solutions for the curve y² = x³ + 17 (the largest solution
 * found is x = 5234). */
static const int64_t LIMIT = 1000000;

/* Temporary buffer used during the first computation. */
static bool _computed = false;
static size_t _sol_count = 0;
static seq_t _solutions[32];

/* Integer square root for unsigned 64‑bit values, using binary search.   */
static unsigned long long isqrt_u64(unsigned long long v)
{
    unsigned long long lo = 0, hi = (1ULL << 32);
    while (lo < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Find all integer solutions once and cache them. */
static void compute_solutions(void)
{
    if (_computed) return;

    /* Temporary dynamic array to gather solutions. */
    size_t cap = 32;
    seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t nsol = 0;

    for (int64_t ix = -LIMIT; ix <= LIMIT; ++ix) {
        __int128 val128 = (__int128)ix * ix * ix + (__int128)17;
        if (val128 < 0) continue;             /* y² must be non‑negative */

        unsigned long long val = (unsigned long long)val128;
        unsigned long long y = isqrt_u64(val);
        if (y * y == val) {                   /* perfect square found */

            seq_t xseq = (seq_t)ix;           /* cast to seq_t – safe
                                                because solution values
                                                are small enough to fit */

            if (nsol == cap) {
                cap *= 2;
                tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
            }
            tmp[nsol++] = xseq;
        }
    }

    /* Sort solutions by Mordell ordering. */
    qsort(tmp, nsol, sizeof(seq_t), cmp_mordell);

    /* Copy to static buffer – the sequence is finite. */
    if (nsol > 32) nsol = 32;                  /* just in case */
    for (size_t i = 0; i < nsol; ++i) {
        _solutions[i] = tmp[i];
    }
    _sol_count = nsol;

    free(tmp);
    _computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for n = 0 */

    compute_solutions();
    if (n - 1 < _sol_count)
        return _solutions[n - 1];
    return 0;                      /* for indices beyond the finite list */
}
