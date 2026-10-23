/*
 * A167468  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, a(4)=1726272,
   a(5)=62145792, a(6)=2237248512, a(7)=80540946432,
   a(8)=2899474071552, a(9)=104381066575872,
   a(10)=3757718396731392, a(11)=135277862282330112,
   a(12)=4870003042163884032, a(13)=175320109517899825152,
   a(14)=6311523942644393704806.
   For n ≥ 15:
       a(n) = 35*∑_{k=1}^{13} a(n‑k) – 630*a(n‑14)   */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Static cache that remembers previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap   = 0;
    static size_t cached_up_to = 0;   /* largest n computed so far */

    /* Allocate and initialise cache on first call. */
    if (cache == NULL) {
        /* Ensure space for at least indices 0‑14. */
        cache_cap = (n >= 14 ? n + 1 : 15);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        /* First 14 terms given as strings. */
        static const char *init_str[14] = {
            "1", "37", "1332", "47952", "1726272",
            "62145792", "2237248512", "80540946432",
            "2899474071552", "104381066575872",
            "3757718396731392", "135277862282330112",
            "4870003042163884032", "175320109517899825152"
        };

        for (size_t i = 0; i < 14; ++i)
            cache[i] = seq_from_str(init_str[i]);

        /* Compute a(14) using the recurrence with +1 constant. */
        seq_t sum = 0;
        for (size_t k = 1; k <= 13; ++k)
            sum += cache[13 - k + 1];          /* a(13) … a(1) */
        seq_t val = ((seq_t)35) * sum;
        val -= ((seq_t)630) * cache[0];
        val += 1;                               /* c_14 */
        cache[14] = val;
        cached_up_to = 14;
    }

    /* Grow the cache if the requested index is not yet allocated. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return immediately if already computed. */
    if (n <= cached_up_to)
        return cache[n];

    /* Extend the cache up to n using the recurrence. */
    for (size_t i = cached_up_to + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 13; ++k)
            sum += cache[i - k];
        seq_t val = ((seq_t)35) * sum;
        val -= ((seq_t)630) * cache[i - 14];
        if (i == 14)          /* the constant term appears only at n = 14 */
            val += 1;
        cache[i] = val;
    }

    cached_up_to = n;
    return cache[n];
}
