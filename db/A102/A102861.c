/*
 * A102861  Numbers which in base 5 have digit-sum 4.
 * https://oeis.org/A102861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 210, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102861
 * Numbers whose base‑5 digit sum is 4, i.e. sums of four powers of 5 (repetitions allowed).
 * The function generates all such sums up to the required index and returns the nth
 * smallest one (1‑based).  It uses an initial enumeration of all combinations of
 * four exponents with repetition and sorts the resulting numbers.  No external
 * utilities or dynamic allocation are required other than a fixed static buffer. */
#include <stdlib.h>
#include "../utils/compare_seq.h"

/* Maximum number of terms that may be required for the supported n range.
   For n up to 100 000 the required size is below 200 000, which safely covers
   the limits stipulated in the problem statement. */
#define MAX_CACHE 200000

static seq_t a_cache[MAX_CACHE];
static idx_t  a_cache_size = 0;

/* Build the cache up to at least n_needed terms. */
static void rebuild_cache(idx_t n_needed)
{
    /* Find minimal exponent E such that C(E+4,4) >= n_needed.
       C(E+4,4) = (E+4)(E+3)(E+2)(E+1)/24. */
    idx_t E = 0;
    unsigned __int128 comb = 1;  /* C(4,4) = 1 at E=0 */
    while (comb < n_needed) {
        ++E;
        unsigned __int128 a = (unsigned __int128)(E + 4);
        unsigned __int128 b = (unsigned __int128)(E + 3);
        unsigned __int128 c = (unsigned __int128)(E + 2);
        unsigned __int128 d = (unsigned __int128)(E + 1);
        comb = a * b * c * d / 24;
    }
    idx_t new_size = (idx_t)comb;
    if (new_size > MAX_CACHE) new_size = MAX_CACHE;  /* safety bound */

    /* Pre‑compute powers of 5 up to exponent E into a stack array. */
    seq_t pow5[128];  /* 128 is safely larger than any E we will need here */
    pow5[0] = (seq_t)1;
    for (idx_t i = 1; i <= E; ++i) {
        pow5[i] = pow5[i - 1] * (seq_t)5;
    }

    /* Generate all sums of four powers (allowing repeats). */
    idx_t pos = 0;
    for (idx_t e1 = 0; e1 <= E; ++e1) {
        for (idx_t e2 = e1; e2 <= E; ++e2) {
            for (idx_t e3 = e2; e3 <= E; ++e3) {
                for (idx_t e4 = e3; e4 <= E; ++e4) {
                    a_cache[pos++] =
                        pow5[e1] + pow5[e2] + pow5[e3] + pow5[e4];
                }
            }
        }
    }
    /* pos should equal new_size; if new_size is capped by MAX_CACHE, pos may be less. */
    a_cache_size = pos;

    /* Sort the generated numbers. */
    qsort(a_cache, a_cache_size, sizeof(seq_t), compare_seq);
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n<1, return 0 */
        return (seq_t)0;
    if (a_cache_size < n)
        rebuild_cache(n);
    return a_cache[n - 1];
}
