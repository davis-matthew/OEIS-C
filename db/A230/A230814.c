/*
 * A230814  Number of nX3 0..2 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value (x(i,j)+1) mod
 *          3 and at least one element with value (x(i,j)-1) mod 3, and upper
 *          left element zero.
 * https://oeis.org/A230814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Empirical recurrence for A230814:
   a(n) = 6*a(n-1) -2*a(n-2) -19*a(n-3) -a(n-4) +169*a(n-5)
          -607*a(n-6) +913*a(n-7) -1279*a(n-8) +2352*a(n-9)
          -3003*a(n-10) +3418*a(n-11) -2775*a(n-12) +1279*a(n-13)
          -676*a(n-14) +156*a(n-15) -4*a(n-16) -62*a(n-17)
          +60*a(n-18) -12*a(n-19)
   with a(1) = 0, a(2)=2, a(3)=4, a(4)=12, a(5)=100, a(6)=326,
   a(7)=2114, a(8)=9226, a(9)=47220, a(10)=233480, a(11)=1135788,
   a(12)=5678402, a(13)=27785526, a(14)=137642146,
   a(15)=678271020, a(16)=3346581344, a(17)=16519160048,
   a(18)=81474120774, a(19)=402103867046. */
   
/* Initial values (indices 1..19); index 0 is unused dummy. */
static const unsigned long long init_vals[20] = {
    0ULL,                 /* dummy 0 for index 0 */
    0ULL, 2ULL, 4ULL, 12ULL, 100ULL, 326ULL, 2114ULL, 9226ULL,
    47220ULL, 233480ULL, 1135788ULL, 5678402ULL, 27785526ULL,
    137642146ULL, 678271020ULL, 3346581344ULL, 16519160048ULL,
    81474120774ULL, 402103867046ULL
};

static seq_t *cache = NULL;       /* cache array holding computed terms     */
static size_t cache_cap = 0;      /* current allocated size (capacity)       */
static idx_t cache_max = 0;       /* highest n that has been computed        */

/* Ensure that the cache can hold terms up to and including n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        size_t init_cap = (n > 64) ? (size_t)n + 16 : 64;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = init_cap;
    }
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize fixed initial values once */
    if (cache_max < 19) {
        for (idx_t i = 1; i <= 19 && i <= n; ++i)
            cache[i] = (seq_t)init_vals[i];
        cache_max = (cache_max < 19) ? 19 : cache_max;
    }
}

/* Compute the n-th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard – sequence is defined for n >= 1 */

    if (n <= 19)
        return (seq_t)init_vals[(size_t)n];

    ensure_cache(n);

    if (n <= cache_max)
        return cache[(size_t)n];

    /* Compute subsequent terms using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions (indices from 1 to 18). */
        pos += (seq_t)6   * cache[(size_t)(i-1)];
        pos += (seq_t)169 * cache[(size_t)(i-5)];
        pos += (seq_t)913 * cache[(size_t)(i-7)];
        pos += (seq_t)2352 * cache[(size_t)(i-9)];
        pos += (seq_t)3418 * cache[(size_t)(i-11)];
        pos += (seq_t)1279 * cache[(size_t)(i-13)];
        pos += (seq_t)156  * cache[(size_t)(i-15)];
        pos += (seq_t)60   * cache[(size_t)(i-18)];

        /* Negative contributions. */
        neg += (seq_t)2    * cache[(size_t)(i-2)];
        neg += (seq_t)19   * cache[(size_t)(i-3)];
        neg += (seq_t)1    * cache[(size_t)(i-4)];
        neg += (seq_t)607  * cache[(size_t)(i-6)];
        neg += (seq_t)1279 * cache[(size_t)(i-8)];
        neg += (seq_t)3003 * cache[(size_t)(i-10)];
        neg += (seq_t)2775 * cache[(size_t)(i-12)];
        neg += (seq_t)676  * cache[(size_t)(i-14)];
        neg += (seq_t)4    * cache[(size_t)(i-16)];
        neg += (seq_t)62   * cache[(size_t)(i-17)];
        neg += (seq_t)12   * cache[(size_t)(i-19)];

        cache[(size_t)i] = pos - neg;
    }

    cache_max = n;
    return cache[(size_t)n];
}
