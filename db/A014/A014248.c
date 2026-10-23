/*
 * A014248  a(n) = b(n) - c(n) where b(n) = [ n*(sqrt(2)+sqrt(3)) ] and c(n)
 *          is the n-th number not in sequence b( ).
 * https://oeis.org/A014248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(sqrt(2)+sqrt(3))) -
 *            floor(n*(sqrt(2)+sqrt(3))/(sqrt(2)+sqrt(3)-1)) (B(n) is the
 *            Beatty sequence for alpha = sqrt(2)+sqrt(3); C(n) is the
 *            complementary Beatty sequence for beta = alpha/(alpha-1).)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 2360, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* Cache of already computed a(i) values */
    static seq_t *cache   = NULL;   /* cache[i] holds a(i) for i <= cache_max */
    static idx_t  cache_max = 0;

    /* If we already have the value, return it immediately. */
    if (n <= cache_max)
        return cache[n];

    /* Compute a(i) up to the requested n (may be larger than cache_max). */
    idx_t  new_max = n;
    size_t arr_size = (size_t)new_max + 1;        /* indices 1..new_max used */

    /* Allocate arrays for b(i) and final a(i). */
    seq_t *b = (seq_t *)calloc(arr_size, sizeof(seq_t));
    if (!b) return (seq_t)0;           /* allocation failed */

    seq_t *new_cache = (seq_t *)calloc(arr_size, sizeof(seq_t));
    if (!new_cache) { free(b); return (seq_t)0; }

    seq_t max_b = 0;      /* keep track of the largest b(i) */

    /* Step 1: compute all b(i) = floor(i*(sqrt(2)+sqrt(3))) */
    for (idx_t i = 1; i <= new_max; ++i)
    {
        seq_t nsq = (seq_t)i * (seq_t)i;            /* i^2 */

        seq_t A = floor_sqrt( (seq_t)2 * nsq );   /* floor(i*sqrt(2)) */
        seq_t B = floor_sqrt( (seq_t)3 * nsq );   /* floor(i*sqrt(3)) */

        seq_t S = A + B + 1;                       /* candidate with possible +1 */

        /* Determine whether to add the missing +1 by checking
           whether the fractional parts of i*sqrt(2) and i*sqrt(3)
           together exceed or equal 1.  This is equivalent to
           checking D^2 < 24 * i^4 where D = S^2 - 5*i^2. */
        seq_t D  = S * S - (seq_t)5 * nsq;
        seq_t D2 = D * D;
        seq_t RHS = (seq_t)24 * nsq * nsq;
        seq_t delta = (D2 < RHS) ? ((seq_t)1) : ((seq_t)0);

        b[i] = A + B + delta;
        if (b[i] > max_b) max_b = b[i];
    }

    /* Step 2: build a bitmap of numbers present in the b sequence. */
    size_t pres_size = (size_t)max_b + 2;          /* +2 to avoid bounds checking */
    bool *present = (bool *)calloc(pres_size, sizeof(bool));
    if (!present) { free(b); free(new_cache); return (seq_t)0; }

    for (idx_t i = 1; i <= new_max; ++i)
        present[(size_t)b[i]] = true;

    /* Step 3: compute a(i) = b(i) - c(i),
       where c(i) is the i-th missing number from the b sequence. */
    idx_t missing = 0;
    for (size_t m = 1; missing < new_max; ++m)
    {
        bool is_missing = (m < pres_size) ? !present[m] : true;
        if (is_missing)
        {
            ++missing;                          /* this is missing index */
            new_cache[missing] = b[missing] - (seq_t)m;  /* a(m) = b(m) - m */
        }
    }

    free(b);
    free(present);

    /* Replace old cache with the new one. */
    if (cache) free(cache);
    cache     = new_cache;
    cache_max = new_max;

    return cache[n];
}
