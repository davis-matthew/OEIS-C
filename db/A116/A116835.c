/*
 * A116835  Number of permutations of length n which avoid the patterns 1234,
 *          2431, 4132.
 * https://oeis.org/A116835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) for n ≥ 15 follows the linear recurrence a(n) = 14 a(n‑1) –
 *            86 a(n‑2) + 305 a(n‑3) – 691 a(n‑4) + 1044 a(n‑5) – 1064 a(n‑6)
 *            + 721 a(n‑7) – 310 a(n‑8) + 76 a(n‑9) – 8 a(n‑10). The first 14
 *            values are taken from the OEIS entry: a(1)=1, a(2)=2, a(3)=6,
 *            a(4)=21, a(5)=71, a(6)=209, a(7)=545, a(8)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc,realloc,free */

#if defined(__cplusplus)
#include <type_traits>
typedef typename std::conditional<
    std::is_unsigned<seq_t>::value,
    __int128,           // use signed 128‑bit for unsigned seq_t
    seq_t
>::type rec_t;
#else
/* In C, seq_t is unsigned (uint8/16/32/64/128), so we use signed __int128 */
typedef signed __int128 rec_t;
#endif

seq_t a(idx_t n)
{
    /* Cache of computed terms (1‑based).  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of entries allocated */

    /* Initial values for n = 1 … 14 */
    static const seq_t init_vals[15] = {
        0,          /* index 0, unused */
        1ULL,       /* a(1) */
        2ULL,       /* a(2) */
        6ULL,       /* a(3) */
        21ULL,      /* a(4) */
        71ULL,      /* a(5) */
        209ULL,     /* a(6) */
        545ULL,     /* a(7) */
        1348ULL,    /* a(8) */
        3270ULL,    /* a(9) */
        7908ULL,    /* a(10) */
        19201ULL,   /* a(11) */
        46918ULL,   /* a(12) */
        115407ULL,  /* a(13) */
        285642ULL   /* a(14) */
    };

    /* Return a known value if requested within the seed interval. */
    if (n <= 14)
        return init_vals[(size_t)n];

    /* Allocate cache on first use.  Size 15 holds the initial values. */
    if (cache == NULL) {
        cache_size = 15;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* initialise cached values (index 0 stays 0) */
        for (idx_t i = 0; i <= 14; ++i)
            cache[i] = init_vals[(size_t)i];
    }

    /* Extend the cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failed – abort */
            abort();
        cache = tmp;

        /* Compute terms up to n.  The recurrence is valid for n ≥ 15. */
        for (idx_t i = cache_size; i <= n; ++i) {
            rec_t val = 0;
            val += (rec_t)14  * cache[i-1];
            val -= (rec_t)86  * cache[i-2];
            val += (rec_t)305 * cache[i-3];
            val -= (rec_t)691 * cache[i-4];
            val += (rec_t)1044* cache[i-5];
            val -= (rec_t)1064* cache[i-6];
            val += (rec_t)721 * cache[i-7];
            val -= (rec_t)310 * cache[i-8];
            val += (rec_t)76  * cache[i-9];
            val -= (rec_t)8   * cache[i-10];
            cache[i] = (seq_t)val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
