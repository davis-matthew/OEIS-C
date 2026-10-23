/*
 * A194646  Number of ways to place 4n nonattacking kings on an 8 X 2n
 *          cylindrical chessboard.
 * https://oeis.org/A194646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -300*a(n-12) + 4235*a(n-11) - 23320*a(n-10) + 66422*a(n-9)
 *               - 111545*a(n-8) + 118727*a(n-7) - 83449*a(n-6) + 39539*a(n-5)
 *               - 12676*a(n-4) + 2708*a(n-3) - 369*a(n-2) + 29*a(n-1)
 *  (with a(1)=80, a(2)=276, …, a(12)=720390254).  Uses memoisation so that
 *  a(n) can be called in any order.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_mul.h"

/* the 12 linear‑recurrence coefficients */
static const int COEFF[12] = {
    29,   -369,   2708,  -12676,  39539,
   -83449, 118727, -111545,  66422,  -23320,
    4235,   -300
};

/* the first 12 terms of the sequence */
static const seq_t BASE[12] = {
    (seq_t)80ULL,        (seq_t)276ULL,        (seq_t)1082ULL,
    (seq_t)4460ULL,      (seq_t)18890ULL,      (seq_t)81606ULL,
    (seq_t)358564ULL,    (seq_t)1599820ULL,    (seq_t)7238864ULL,
    (seq_t)33175486ULL,  (seq_t)153802520ULL,  (seq_t)720390254ULL
};

static seq_t *CACHE = NULL;      /* 1‑based cache of computed values   */
static size_t CACHE_SIZE = 0;    /* number of allocated slots (incl. 0) */
static size_t CACHE_LEN  = 0;    /* highest index that has been computed */

/* Allocate or enlarge the cache so that it can hold up to index n (inclusive). */
static void ensure_cache(size_t n)
{
    if (n + 1 <= CACHE_SIZE)
        return;

    size_t new_size = (n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(CACHE, new_size * sizeof(seq_t));
    if (tmp == NULL)            /* should never happen in tests */
        abort();
    CACHE = tmp;
    CACHE_SIZE = new_size;

    /* Initialise the cache with the known first 12 terms, if not yet done. */
    if (CACHE_LEN < 12) {
        for (size_t i = 1; i <= 12; ++i)
            CACHE[i] = BASE[i - 1];
        CACHE_LEN = 12;
    }
}

/* Compute the n‑th term (n ≥ 1).  The function is safe for repeated calls
 * with any ordering of n. */
seq_t a(idx_t n)
{
    if (n <= 12)
        return BASE[(size_t)n - 1];

    ensure_cache((size_t)n);

    if ((size_t)n <= CACHE_LEN)
        return CACHE[(size_t)n];

    /* Compute values up to n, starting after the last computed index. */
    for (size_t i = CACHE_LEN + 1; i <= (size_t)n; ++i) {
        seq_t pos = 0;    /* sum of terms with positive coefficient */
        seq_t neg = 0;    /* sum of terms with negative coefficient */

        for (int k = 0; k < 12; ++k) {
            int c = COEFF[k];
            seq_t prod = seq_mul(CACHE[i - (size_t)(k + 1)],
                                 (seq_t)(c >= 0 ? c : -c));
            if (c >= 0)
                pos += prod;
            else
                neg += prod;
        }
        CACHE[i] = pos - neg;   /* pos ≥ neg by the recurrence */
    }

    CACHE_LEN = (size_t)n;
    return CACHE[(size_t)n];
}
