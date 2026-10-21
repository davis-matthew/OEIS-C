/*
 * A075919  Sixth column of triangle A075501.
 * https://oeis.org/A075919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * S2(n+6, 6) where S2 is Stirling numbers of the
 *            second kind (OEIS A075919 – Sixth column of triangle A075501)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Helper: multiply a seq_t by a small integer (1–10).  Works for native integer
   types and for arbitrary‑precision types that support the * operator. */
static seq_t mul_small(seq_t x, unsigned int k)
{
    return x * (seq_t)k;
}

seq_t a(idx_t n)
{
    /* ---------- cached values ---------- */
    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t cache_size = 0;    /* number of cached entries */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* ---------- current Stirling row S(m, k) for k=0..6 ---------- */
    static seq_t st[7];
    static idx_t cur_m = 0;          /* current m (equals cached index + 6) */
    static seq_t pow6 = 1;           /* 6^current cached index */

    /* initialisation on first call */
    if (!cache)
    {
        cache_cap  = 16;
        cache_size = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        /* S(0,0)=1, all other S(0,k)=0 */
        st[0] = 1;
        for (int i = 1; i < 7; ++i)
            st[i] = 0;
        cur_m  = 0;
        pow6   = 1;
    }

    /* grow the cache until it contains a(n) */
    while (cache_size <= n)
    {
        /* need Stirling numbers for m = cache_size + 6 */
        idx_t target_m = cache_size + 6;

        /* advance the Stirling row from cur_m+1 to target_m */
        for (idx_t m = cur_m + 1; m <= target_m; ++m)
        {
            int km = (m < 6) ? (int)m : 6;   /* we only need k <= 6 */

            /* update higher k first to avoid overwriting */
            for (int k = km; k >= 1; --k)
                st[k] = mul_small(st[k], (unsigned int)k) + st[k - 1];

            st[0] = 0;   /* S(m,0)=0 for m>0 */
        }
        cur_m = target_m;

        /* compute a(cache_size) = 6^cache_size * S(cache_size+6, 6) */
        seq_t val = pow6 * st[6];

        /* store the value */
        if (cache_size == cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size] = val;
        ++cache_size;

        /* update 6^next_index for the following iteration */
        pow6 = mul_small(pow6, 6);
    }

    return cache[n];
}
