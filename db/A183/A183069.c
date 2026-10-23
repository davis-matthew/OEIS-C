/*
 * A183069  L.g.f.: Sum_{n>=1,k>=0} CATALAN(n,k)^2 * x^(n+k)/n = Sum_{n>=1}
 *          a(n)*x^n/n, where CATALAN(n,k) = n*C(n+2*k-1,k)/(n+k) is the
 *          coefficient of x^k in C(x)^n and C(x) is the g.f. of the Catalan
 *          numbers.
 * https://oeis.org/A183069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183069
 *  Recurrence (with initial values):
 *      a(1) = 1, a(2) = 3
 *      2*n^2*(2*n-1)*(7*n^2-20*n+14)*a(n) =
 *          (455*n^5-2427*n^4+4850*n^3-4406*n^2+1728*n-216)*a(n-1)
 *          - 4*(n-2)*(2*n-3)^2*(7*n^2-6*n+1)*a(n-2)
 */

#include <stdlib.h>
#include <stddef.h>

/* ----------  cache  ---------- */
static seq_t *cache = NULL;      /* array of computed terms            */
static size_t cache_cap = 0;     /* allocated capacity of cache array   */
static idx_t cache_max = 0;      /* largest index that has been computed */

/* initialise the cache and store the first two values   */
static void init_cache(void)
{
    if (!cache)
    {
        cache_cap = 4;          /* enough for indices 0 … 3          */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)0;   /* unused                            */
        cache[1]  = (seq_t)1;   /* a(1)                              */
        cache[2]  = (seq_t)3;   /* a(2)                              */
        cache[3]  = (seq_t)0;   /* placeholder for future entries    */
        cache_max = 2;          /* largest known index               */
    }
}

/* ----------  compute for small seq_t (≤ 8 bytes)  ---------- */
static void compute_small(idx_t from, idx_t to)
{
    for (idx_t i = from; i <= to; ++i)
    {
        if ((size_t)i + 1 > cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)i + 1) new_cap = (size_t)i + 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        unsigned __int128 N  = (unsigned __int128)i;
        unsigned __int128 N2 = N * N;
        unsigned __int128 N3 = N2 * N;
        unsigned __int128 N4 = N3 * N;
        unsigned __int128 N5 = N4 * N;

        unsigned __int128 coeff1 = (unsigned __int128)455 * N5
                                 - (unsigned __int128)2427 * N4
                                 + (unsigned __int128)4850 * N3
                                 - (unsigned __int128)4406 * N2
                                 + (unsigned __int128)1728 * N
                                 - (unsigned __int128)216;
        unsigned __int128 coeff2 = (unsigned __int128)4 * (N - 2)
                                 * ((unsigned __int128)2 * N - 3)
                                 * ((unsigned __int128)2 * N - 3)
                                 * ((unsigned __int128)7 * N2
                                    - (unsigned __int128)6 * N + 1);
        unsigned __int128 denom = (unsigned __int128)2 * N2
                                 * ((unsigned __int128)2 * N - 1)
                                 * ((unsigned __int128)7 * N2
                                    - (unsigned __int128)20 * N + 14);

        unsigned __int128 term1 = coeff1 * (unsigned __int128)cache[i-1];
        unsigned __int128 term2 = coeff2 * (unsigned __int128)cache[i-2];
        unsigned __int128 numer = term1 - term2;
        unsigned __int128 an128 = numer / denom;   /* exact division */

        cache[i] = (seq_t)an128;   /* truncated to seq_t width              */
    }
}

/* ----------  compute for arbitrary-precision seq_t  ---------- */
static void compute_big(idx_t from, idx_t to)
{
    for (idx_t i = from; i <= to; ++i)
    {
        if ((size_t)i + 1 > cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)i + 1) new_cap = (size_t)i + 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        seq_t N  = (seq_t)i;
        seq_t N2 = N * N;
        seq_t N3 = N2 * N;
        seq_t N4 = N3 * N;
        seq_t N5 = N4 * N;

        seq_t coeff1 = (seq_t)455 * N5
                     - (seq_t)2427 * N4
                     + (seq_t)4850 * N3
                     - (seq_t)4406 * N2
                     + (seq_t)1728 * N
                     - (seq_t)216;
        seq_t coeff2 = (seq_t)4 * (N - (seq_t)2)
                     * ((seq_t)2 * N - (seq_t)3)
                     * ((seq_t)2 * N - (seq_t)3)
                     * ((seq_t)7 * N2 - (seq_t)6 * N + (seq_t)1);
        seq_t denom = (seq_t)2 * N2
                     * ((seq_t)2 * N - (seq_t)1)
                     * ((seq_t)7 * N2 - (seq_t)20 * N + (seq_t)14);

        seq_t term1 = coeff1 * cache[i-1];
        seq_t term2 = coeff2 * cache[i-2];
        seq_t numer = term1 - term2;
        seq_t an    = numer / denom;   /* exact division */

        cache[i] = an;
    }
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    init_cache();

    if (n <= cache_max) return cache[n];

    /* choose computation path based on seq_t size */
#ifdef __SIZEOF_INT128__
    if (sizeof(seq_t) <= 8)
        compute_small(cache_max + 1, n);
    else
        compute_big(cache_max + 1, n);
#else
    /* if __int128 is not available, fall back to arbitrary-precision path */
    compute_big(cache_max + 1, n);
#endif

    cache_max = n;
    return cache[n];
}
