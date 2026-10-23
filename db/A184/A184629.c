/*
 * A184629  Floor(1/{(5+n^4)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/{(5 + n^4)^(1/4)}), n≥1 Recurrence for n≥15:
 *            a(n)=3a(n-1)-3a(n-2)+a(n-3)+a(n-5)-3a(n-6)+3a(n-7)-a(n-8)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 1751, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t compute_direct(idx_t n)
{
    /* compute a(n) for n≤14 using binary search on k:
       floor(1/t) where t = (5+n^4)^(1/4) - n
       Equivalent to largest k with (n*k+1)^4 ≥ (n^4+5)*k^4
    */
    unsigned __int128 nn = n;
    unsigned __int128 n4 = nn * nn * nn * nn;
    unsigned __int128 target = n4 + 5;          /* n^4 + 5  */

    /*  high bound large enough for n≤14 (a(14)=2195) */
    unsigned int low = 1, high = 4000, mid, ans = 0;

    while (low <= high)
    {
        mid = low + (high - low) / 2;
        unsigned __int128 nk1 = nn * mid + 1;
        unsigned __int128 left  = nk1 * nk1 * nk1 * nk1;   // (n*k+1)^4

        unsigned __int128 k4   = (unsigned __int128)mid * mid * mid * mid;   // k^4
        unsigned __int128 right = target * k4;                          // (n^4+5)*k^4

        if (left >= right)
        {
            ans = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return (seq_t)ans;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static idx_t  max_n   = 0;      /* highest index computed */

    if (n == 0)
        return 0;   /* not used, but guards the array */

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size)
    {
        size_t new_size = (size_t)n + 2;          /* +2 to be safe */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                       /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (n <= max_n)
        return cache[n];

    /* Compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        if (i <= 14)                  /* small indices – compute directly */
        {
            cache[i] = compute_direct(i);
        }
        else                          /* recurrence for n≥15 */
        {
            __int128 val = 0;
            val += 3 * (__int128)cache[i-1];
            val -= 3 * (__int128)cache[i-2];
            val += (__int128)cache[i-3];
            val += (__int128)cache[i-5];
            val -= 3 * (__int128)cache[i-6];
            val += 3 * (__int128)cache[i-7];
            val -= (__int128)cache[i-8];
            cache[i] = (seq_t)val;
        }
    }
    max_n = n;
    return cache[n];
}
