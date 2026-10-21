/*
 * A094873  n! has a triangular number of digits.
 * https://oeis.org/A094873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th nonnegative integer m such that the number of
 *            digits of m! is a triangular number (1, 3, 6, 10, …). Offset: 0
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Integer square‑root for 64‑bit values. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0 || n == 1) return n;
    uint64_t x = (uint64_t)sqrtl((long double)n);
    while (x * x > n) --x;
    while ((x + 1) * (x + 1) <= n) ++x;
    return x;
}

/* Test whether a positive integer d is triangular:
   8*d + 1 must be an odd perfect square.  */
static inline bool is_triangular(uint64_t d)
{
    uint64_t v = 8ULL * d + 1ULL;
    uint64_t s = isqrt_u64(v);
    return s * s == v;
}

/* Sequence a(n).  The function builds an internal cache of the
   qualifying indices on demand, so calls may arrive in any order. */
seq_t a(idx_t n)
{
    /* Static cache of qualifying indices. */
    static seq_t *qual = NULL;
    static size_t qual_cap = 0;
    static size_t qual_len = 0;

    /* State for incremental factorial digit counting. */
    static idx_t next_factorial = 1;     /* next factorial to evaluate */
    static long double log_sum = 0.0L;    /* sum of log10(i) for i<next_factorial */

    /* Initialise cache on first use. */
    if (qual == NULL) {
        qual_cap = 64;
        qual = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        if (!qual) abort();              /* allocation failure */
        /* 0! = 1, 1 digit -> triangular, so a(0)=0 */
        qual[0] = (seq_t)0;
        qual_len = 1;
    }

    /* Extend cache until we have at least (n+1) qualifying indices. */
    while (qual_len <= (size_t)n) {
        /* Incrementally update log_sum for next_factorial!. */
        log_sum += log10l((long double)next_factorial);

        /* Number of decimal digits of next_factorial!. */
        uint64_t digits = (uint64_t)log_sum + 1ULL;

        if (is_triangular(digits)) {
            /* Grow cache if necessary. */
            if (qual_len == qual_cap) {
                qual_cap = qual_cap * 2;
                seq_t *tmp = (seq_t *)realloc(qual, qual_cap * sizeof(seq_t));
                if (!tmp) abort();
                qual = tmp;
            }
            /* Store the qualifying index. */
            qual[qual_len++] = (seq_t)next_factorial;
        }

        ++next_factorial;
    }

    return qual[(size_t)n];
}
