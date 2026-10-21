/*
 * A098714  Only one Pythagorean triangle of this perimeter exists.
 * https://oeis.org/A098714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_vector
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098714 – Only one Pythagorean triangle of this perimeter exists.
 *  The routine generates all primitive Pythagorean triples by Euclid's
 *  formula  a=m²-n², b=2mn, c=m²+n²  (m>n>0, m-n odd, gcd(m,n)=1).
 *  For every primitive perimeter p₀ = 2m(m+n) all integer multiples k·p₀
 *  are counted.  After the count array is built we collect those perimeters
 *  that appear exactly once – those are the desired terms of the sequence.
 *  The build is performed once (static initialization), and the cached
 *  list is indexed by the sequence position.  The algorithm runs in
 *  O(LIMIT log log LIMIT) time with LIMIT chosen large enough to cover
 *  the first 10 000 terms and uses only dynamic memory operations and
 *  integer arithmetic.  No external libraries or floating point are used.
 */

#include <stdlib.h>   /* malloc, free, calloc */
#include <stdbool.h>  /* bool type */
#include "../utils/grow_vector.h"

/* ----- helper: integer gcd, works for any unsigned integer type ----- */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----- static cache of unique perimeters ----- */
static seq_t   *unique_perims = NULL;
static size_t  unique_count  = 0;
static size_t  unique_cap    = 0;
static bool    cache_built   = false;

/* ----- build the cache once ----- */
static void init_sequence(void)
{
    if (cache_built) return;
    cache_built = true;

    /* target: more than 10 000 unique perimeters.  10 000 000 gives comfortably
     * more than 30 000 unique values and fits easily in memory for the
     * auxiliary count array (≈20 MB when using uint16_t). */
    const idx_t LIMIT = 10000000UL;

    /* count array: uint16_t suffices as a multiplier count never exceeds 65535
     * for this LIMIT.  Allocate with calloc to initialise to zero. */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(LIMIT + 1), sizeof(uint16_t));
    if (!cnt) return;  /* allocation failure – give up silently */

    /* upper bound for m: 2m(m+n) ≤ LIMIT  ⇒  m² ≤ LIMIT/2  */
    idx_t mmax = 1;
    while ((mmax + 1) * (mmax + 1) <= LIMIT / 2) ++mmax;

    for (idx_t m = 2; m <= mmax; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            /* parity: one odd, one even */
            if (((m - n) & 1U) == 0) continue;
            if (gcd_seq_t((seq_t)m, (seq_t)n) != (seq_t)1) continue;

            idx_t p0 = 2 * m * (m + n);        /* primitive perimeter */
            if (p0 > LIMIT) break;            /* larger n increase p0 */

            for (idx_t k = 1; k * p0 <= LIMIT; ++k) {
                ++cnt[k * p0];
            }
        }
    }

    /* collect perimeters that appear exactly once */
    unique_cap = 256;                          /* initial capacity */
    unique_perims = (seq_t *)malloc(unique_cap * sizeof(seq_t));
    if (!unique_perims) { free(cnt); return; } /* allocation failure */

    for (idx_t p = 12; p <= LIMIT; ++p) {     /* smallest perimeter is 12 */
        if (cnt[p] == 1) {
            if (unique_count >= unique_cap)
                grow_vector(&unique_perims, &unique_count, &unique_cap,
                            unique_count + 16);
            unique_perims[unique_count++] = (seq_t)p;
        }
    }

    free(cnt);   /* free auxiliary array */
    /* In a production setting we might shrink the vector to the exact size,
     * but this is not required for the task constraints. */
}

/* ----- public routine required by the framework ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined – harmless for callers */
    init_sequence();
    if (n > unique_count) return 0;  /* out of range – safe fallback */
    return unique_perims[n - 1];
}
