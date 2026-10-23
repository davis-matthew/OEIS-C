/*
 * A042493  Denominators of continued fraction convergents to sqrt(774).
 * https://oeis.org/A042493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 20810*a(n-10) - a(n-20)
   Initial values a(0)=1, a(1)=1, a(2)=5, a(3)=6, a(4)=11, a(5)=28,
   a(6)=39, a(7)=67, a(8)=307, a(9)=374, a(10)=20503,
   a(11)=20877, a(12)=104011, a(13)=124888, a(14)=228899,
   a(15)=582686, a(16)=811585, a(17)=1394271, a(18)=6388669,
   a(19)=7782940 */

#include <stdlib.h>

/* Cache for already computed terms */
static seq_t *cache      = NULL;   /* array of seq_t */
static idx_t  cache_len  = 0;      /* one past the last computed index */
static size_t cache_cap  = 0;      /* allocated number of elements */

/* Ensure the cache can hold indices up to 'needed' (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_len)
        return;                    /* already enough */

    if (needed >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 32;
        while (new_cap <= (size_t)needed)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the first 20 terms */
static void init_initial(void)
{
    if (cache_len >= 20)
        return;                    /* already initialised */

    ensure_capacity(19);            /* need room for indices 0..19 */

    cache[0]  = (seq_t)1;
    cache[1]  = (seq_t)1;
    cache[2]  = (seq_t)5;
    cache[3]  = (seq_t)6;
    cache[4]  = (seq_t)11;
    cache[5]  = (seq_t)28;
    cache[6]  = (seq_t)39;
    cache[7]  = (seq_t)67;
    cache[8]  = (seq_t)307;
    cache[9]  = (seq_t)374;
    cache[10] = (seq_t)20503;
    cache[11] = (seq_t)20877;
    cache[12] = (seq_t)104011;
    cache[13] = (seq_t)124888;
    cache[14] = (seq_t)228899;
    cache[15] = (seq_t)582686;
    cache[16] = (seq_t)811585;
    cache[17] = (seq_t)1394271;
    cache[18] = (seq_t)6388669;
    cache[19] = (seq_t)7782940;

    cache_len = 20;
}

seq_t a(idx_t n)
{
    init_initial();

    if (n < cache_len)
        return cache[n];

    ensure_capacity(n);

    /* Compute terms from cache_len up to n using the recurrence
       (valid for indices >= 20) */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 20810*a(i-10) - a(i-20) */
        cache[i] = (seq_t)(
            (unsigned __int128)20810 * cache[i - 10] - cache[i - 20]
        );
    }
    cache_len = n + 1;
    return cache[n];
}
