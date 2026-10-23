/*
 * A166617  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166617: Number of reduced words of length n in
 * a Coxeter group of 30 generators with relations
 * (S_i)^2 = (S_i S_j)^12 = I.
 *
 * The sequence satisfies the following linear recurrence:
 *
 *   a(n) = 28 * (a(n-1)+a(n-2)+...+a(n-11)) - 406 * a(n-12)
 *   for n ≥ 13,
 *
 * with the following initial values (n = 0 … 12):
 *
 *   a(0)  = 1
 *   a(1)  = 30
 *   a(2)  = 870
 *   a(3)  = 25230
 *   a(4)  = 731670
 *   a(5)  = 21218430
 *   a(6)  = 615334470
 *   a(7)  = 17844699630
 *   a(8)  = 517496289270
 *   a(9)  = 15007392388830
 *   a(10) = 435214379276070
 *   a(11) = 12621216999006030
 *   a(12) = 366015292971174435
 *
 * The initial values are derived from the generating function
 * given in the OEIS entry and are required to generate all
 * further terms via the recurrence.
 *
 * The implementation uses memoisation and grows the cache
 * only as needed; all arithmetic is performed using seq_t,
 * which is guaranteed to be a suitable integer type for the
 * build (unsigned 8, 16, 32, 64, 128 bit or an arbitrary‑precision
 * integer class).
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute the nth term of sequence A166617. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;  /* next free index in cache */
    size_t need = (size_t)n;

    /* Initialise the cache on first call. */
    if (!cache) {
        /* Store the first 13 terms explicitly.  The term at n=12
         * is computed from the recurrence with the numerator
         * coefficients, ensuring no typo. */
        static const seq_t init_0_to_11[12] = {
            (seq_t)1,
            (seq_t)30,
            (seq_t)870,
            (seq_t)25230,
            (seq_t)731670,
            (seq_t)21218430,
            (seq_t)615334470,
            (seq_t)17844699630ULL,
            (seq_t)517496289270ULL,
            (seq_t)15007392388830ULL,
            (seq_t)435214379276070ULL,
            (seq_t)12621216999006030ULL
        };

        /* Estimate an initial cache size.  We need at least 13 slots. */
        cap = 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory; caller should handle */

        /* Copy the known values. */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_0_to_11[i];

        /* Compute a(12) from the recurrence with the numerator
         * contribution p(12) = 1. */
        seq_t sum = 0;
        for (size_t i = 1; i <= 11; ++i)
            sum += cache[i];
        cache[12] = (seq_t)1 + (seq_t)28 * sum - (seq_t)406 * cache[0];

        len = 13;  /* cache[0..12] are valid */
    }

    /* Grow the cache if required. */
    if (need >= cap) {
        size_t new_cap = cap;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Generate terms up to n if not already cached. */
    while (len <= need) {
        /* For n ≥ 13 the recurrence is homogeneous. */
        seq_t sum = 0;
        for (size_t k = 1; k <= 11; ++k)
            sum += cache[len - k];
        cache[len] = (seq_t)28 * sum - (seq_t)406 * cache[len - 12];
        ++len;
    }

    return cache[need];
}
