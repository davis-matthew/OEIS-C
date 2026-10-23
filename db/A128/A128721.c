/*
 * A128721  Number of UUU's in all skew Dyck paths of semilength n.
 * https://oeis.org/A128721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of UUU's in all skew Dyck paths of semilength n
 *            (from OEIS A128721). Sequence is 0,0,0,4,28,157,820,4155,20742,
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses an explicit recurrence which is
   valid for n ≥ 4.  The first four terms are hard‑coded and the
   remaining terms are computed by
   2*(n+1)*(121*n-348)*a(n) =
          (1663*n^2 - 4620*n + 1392)*a(n-1)
        - (2476*n^2 - 11133*n + 11787)*a(n-2)
        + 5*(n-4)*(211*n-537)*a(n-3).  */

/* Cache may be updated in any order of calls; results are memoised */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically allocated cache */
static size_t cache_cap = 0;         /* current size of cache */

/* Ensure cache contains values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                   /* already computed */

    size_t old = cache_cap;
    size_t new_cap = (size_t)(n + 1);

    /* enlarge cache */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;

    /* initialise first four terms */
    for (size_t i = old; i < new_cap; ++i)
    {
        if (i <= 2)
        {
            cache[i] = 0;
        }
        else if (i == 3)
        {
            cache[i] = 4;
        }
        else /* i >= 4: use the recurrence */
        {
#ifdef __cplusplus
            /* 64‑bit or wider integer type: use seq_t for all arithmetic */
            seq_t nn   = (seq_t)i;
            seq_t cA   = (seq_t)1663 * nn * nn - (seq_t)4620 * nn + (seq_t)1392;
            seq_t cB   = (seq_t)2476 * nn * nn - (seq_t)11133 * nn + (seq_t)11787;
            seq_t cC   = (seq_t)5 * (nn - 4) * ((seq_t)211 * nn - (seq_t)537);
            seq_t den  = 2 * (nn + 1) * ((seq_t)121 * nn - (seq_t)348);

            seq_t t1   = cA * cache[i - 1];
            seq_t t2   = cB * cache[i - 2];
            seq_t t3   = cC * cache[i - 3];
            seq_t num  = t1 - t2 + t3;
            cache[i]   = num / den;
#else
            /* pure C: use 128‑bit intermediate to avoid overflow */
            __int128 nn   = ( __int128 )i;
            __int128 cA   = (__int128)1663 * nn * nn - (__int128)4620 * nn + (__int128)1392;
            __int128 cB   = (__int128)2476 * nn * nn - (__int128)11133 * nn + (__int128)11787;
            __int128 cC   = (__int128)5 * (nn - 4) * ((__int128)211 * nn - (__int128)537);
            __int128 den  = 2 * (nn + 1) * ((__int128)121 * nn - (__int128)348);

            __int128 t1  = cA * (__int128)cache[i - 1];
            __int128 t2  = cB * (__int128)cache[i - 2];
            __int128 t3  = cC * (__int128)cache[i - 3];
            __int128 num = t1 - t2 + t3;
            cache[i] = (seq_t)(num / den);
#endif
        }
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
