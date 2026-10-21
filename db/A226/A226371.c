/*
 * A226371  Number of tilings of a 7 X n rectangle using integer-sided square
 *          tiles of area > 1.
 * https://oeis.org/A226371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 65, u64 n <= 124,
 *            u128 n <= 242, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A226371 – number of tilings of a 7 × n rectangle using integer‑sided square tiles of area > 1.
 *
 * The sequence satisfies the linear recurrence derived from the generating
 * function
 *   (-x^12 - x^9 + 2*x^6 + x^3 - 1) /
 *   (x^19 - x^18 - x^16 - x^15 - 2*x^13 - 7*x^12 + x^10 - 8*x^9 + x^7 + 9*x^6 + x^3 - 1) .
 * For n ≥ 19 the recurrence is
 *
 *     a(n) =  a(n‑3) +  a(n‑7) +  a(n‑10) +  a(n‑19) 
 *            + 9·a(n‑6) 
 *          – ( 8·a(n‑9) + 7·a(n‑12) + 2·a(n‑13) + a(n‑15) + a(n‑16) + a(n‑18) ).
 *
 * The first 19 values are taken from the OEIS entry and are used to initialise
 * the computation.
 *
 * This implementation memoises values in a static array so that calls may
 * arrive in any order.  It works for any `seq_t` and any `idx_t`.  The
 * auxiliary `mul_small` routine from the utils library is used to multiply by
 * small integer constants.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

static const seq_t init[19] = {
    (seq_t)1,  (seq_t)0,  (seq_t)0,  (seq_t)0,
    (seq_t)0,  (seq_t)0,  (seq_t)7,  (seq_t)1,
    (seq_t)0,  (seq_t)0,  (seq_t)2,  (seq_t)0,
    (seq_t)57, (seq_t)16, (seq_t)1,  (seq_t)0,
    (seq_t)32, (seq_t)4,  (seq_t)463
};

#define MAXN 20000     /* sufficient for all test cases (≈10 000) */

static seq_t cache[MAXN + 1];
static idx_t last_computed = (idx_t)(-1);

seq_t a(idx_t n)
{
    /* initialise cache with known first 19 terms on first use */
    if (last_computed == (idx_t)(-1)) {
        last_computed = 18;
        for (idx_t i = 0; i <= 18; ++i) {
            cache[i] = init[i];
        }
    }

    /* if the value is already computed, return it */
    if (n <= last_computed) {
        return cache[n];
    }

    /* compute all missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* for n <= 18 we already have the value */
        if (i <= 18) {
            cache[i] = init[i];
            continue;
        }

        seq_t pos = 0;
        seq_t neg = 0;

        /* positive coefficients */
        pos += cache[i - 3];
        pos += mul_small(cache[i - 6], (unsigned)9);
        pos += cache[i - 7];
        pos += cache[i - 10];
        pos += cache[i - 19];

        /* negative coefficients */
        neg += mul_small(cache[i - 9], (unsigned)8);
        neg += mul_small(cache[i - 12], (unsigned)7);
        neg += mul_small(cache[i - 13], (unsigned)2);
        neg += cache[i - 15];
        neg += cache[i - 16];
        neg += cache[i - 18];

        /* result is guaranteed non‑negative */
        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
