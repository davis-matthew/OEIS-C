/*
 * A189328  Number of nondecreasing arrangements of 5 numbers in 0..n with the
 *          last equal to n and each after the second equal to the sum of one
 *          or two of the preceding four.
 * https://oeis.org/A189328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence (order 14) a(1)=2, a(2)=8, a(3)=11,
 *            a(4)=20, a(5)=21, a(6)=36, a(7)=31, a(8)=49, a(9)=42, a(10)=63,
 *            a(11)=51, a(12)=79, a(13)=60, a(14)=93 a(n) = -3*a(n-1)
 *            -5*a(n-2) -5*a(n-3) -2*a(n-4) +3*a(n-5) +8*a(n-6) +10*a(n-7)
 *            +8*a(n-8) +3*a(n-9) -2*a(n-10) -5*a(n-11) -5*a(n-12)...
 * Range:     u8 n <= 35, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* stored terms, 0‑based */
static idx_t cache_len = 0;          /* number of stored terms */

/* Return a(n) as defined above.  The function is thread‑safe only for
   sequential calls; parallel calls may race on the static cache. */
seq_t a(idx_t n)
{
    /* Return early for n within the cache */
    if (n <= cache_len)
        return seq_cache[n - 1];

    /* Initialise cache with first 14 terms on first call */
    if (cache_len == 0)
    {
        cache_len = 14;
        seq_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!seq_cache) abort();                  /* fatal allocation error */

        seq_cache[0] =  (seq_t)2;   /* a(1) */
        seq_cache[1] =  (seq_t)8;   /* a(2) */
        seq_cache[2] =  (seq_t)11;  /* a(3) */
        seq_cache[3] =  (seq_t)20;  /* a(4) */
        seq_cache[4] =  (seq_t)21;  /* a(5) */
        seq_cache[5] =  (seq_t)36;  /* a(6) */
        seq_cache[6] =  (seq_t)31;  /* a(7) */
        seq_cache[7] =  (seq_t)49;  /* a(8) */
        seq_cache[8] =  (seq_t)42;  /* a(9) */
        seq_cache[9] =  (seq_t)63;  /* a(10) */
        seq_cache[10] = (seq_t)51;  /* a(11) */
        seq_cache[11] = (seq_t)79;  /* a(12) */
        seq_cache[12] = (seq_t)60;  /* a(13) */
        seq_cache[13] = (seq_t)93;  /* a(14) */
    }

    /* Expand cache to contain terms up to n */
    if (n > cache_len)
    {
        seq_t *new_cache =
            (seq_t *)realloc(seq_cache, n * sizeof(seq_t));
        if (!new_cache) abort();                  /* fatal allocation error */

        seq_cache = new_cache;

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* Compute positive part */
            seq_t pos =
                seq_cache[i - 5 - 1] * (seq_t)3
              + seq_cache[i - 6 - 1] * (seq_t)8
              + seq_cache[i - 7 - 1] * (seq_t)10
              + seq_cache[i - 8 - 1] * (seq_t)8
              + seq_cache[i - 9 - 1] * (seq_t)3;

            /* Compute negative part */
            seq_t neg =
                seq_cache[i - 1 - 1] * (seq_t)3
              + seq_cache[i - 2 - 1] * (seq_t)5
              + seq_cache[i - 3 - 1] * (seq_t)5
              + seq_cache[i - 4 - 1] * (seq_t)2
              + seq_cache[i - 10 - 1] * (seq_t)2
              + seq_cache[i - 11 - 1] * (seq_t)5
              + seq_cache[i - 12 - 1] * (seq_t)5
              + seq_cache[i - 13 - 1] * (seq_t)3
              + seq_cache[i - 14 - 1];

            /* a(i) = pos - neg   (pos ≥ neg for all i ≥ 15) */
            seq_cache[i - 1] = pos - neg;
        }

        cache_len = n;
    }

    return seq_cache[n - 1];
}
