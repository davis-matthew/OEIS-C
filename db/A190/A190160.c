/*
 * A190160  Number of peakless Motzkin paths of length n containing no
 *          subwords of type uh^ju or dh^jd (j>=1), where u=(1,1), h=(1,0),
 *          and d=(1,-1) (can be easily expressed using RNA secondary
 *          structure terminology).
 * https://oeis.org/A190160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A190160:
   (n+2)*a(n) = (5*n+6)*a(n-1)
                -2*(4*n+3)*a(n-2)
                + (2*n+9)*a(n-3)
                + (8*n-25)*a(n-4)
                -6*(n-2)*a(n-5)
                -9*(n-7)*a(n-6)
                + (15*n-96)*a(n-7)
                -7*(n-7)*a(n-8)
                + (n-8)*a(n-9)
   Initial values:
   a(0)=1,a(1)=1,a(2)=1,a(3)=2,a(4)=4,a(5)=8,a(6)=15,a(7)=28,a(8)=53,a(9)=102.
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t computed_up = 9;       /* highest index computed so far */

/* ensure the cache has space for at least size elements */
static void ensure_capacity(size_t size)
{
    if (size < cache_cap)
        return;
    size_t new_cap = cache_cap? cache_cap * 2 : 16;
    while (new_cap <= size)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_capacity(10);
    /* initial terms a(0) .. a(9) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)4;
    cache[5] = (seq_t)8;
    cache[6] = (seq_t)15;
    cache[7] = (seq_t)28;
    cache[8] = (seq_t)53;
    cache[9] = (seq_t)102;
    computed_up = 9;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n <= computed_up)
        return cache[n];

    /* fill cache up to n */
    ensure_capacity((size_t)n + 1);
    for (size_t i = computed_up + 1; i <= (size_t)n; ++i)
    {
        /* coefficients (all positive for n >= 10) */
        seq_t c1 = (seq_t)(5 * i + 6);
        seq_t c2 = (seq_t)(2 * (4 * i + 3));
        seq_t c3 = (seq_t)(2 * i + 9);
        seq_t c4 = (seq_t)(8 * i - 25);
        seq_t c5 = (seq_t)(6 * (i - 2));
        seq_t c6 = (seq_t)(9 * (i - 7));
        seq_t c7 = (seq_t)(15 * i - 96);
        seq_t c8 = (seq_t)(7 * (i - 7));
        seq_t c9 = (seq_t)(i - 8);

        /* positive terms */
        seq_t pos = c1 * cache[i - 1];
        pos += c3 * cache[i - 3];
        pos += c4 * cache[i - 4];
        pos += c7 * cache[i - 7];
        pos += c9 * cache[i - 9];

        /* negative terms */
        seq_t neg = c2 * cache[i - 2];
        neg += c5 * cache[i - 5];
        neg += c6 * cache[i - 6];
        neg += c8 * cache[i - 8];

        /* numerator = pos - neg */
        seq_t numer = pos - neg;
        cache[i] = numer / ((seq_t)i + 2);
    }
    computed_up = (size_t)n;
    return cache[n];
}
