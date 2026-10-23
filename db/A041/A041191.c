/*
 * A041191  Denominators of continued fraction convergents to sqrt(106).
 * https://oeis.org/A041191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 8010*a(n-9) + a(n-18); Initial values: a(0)=1, a(1)=3,
 *            a(2)=7, a(3)=10, a(4)=17, a(5)=27, a(6)=44, a(7)=115, a(8)=389,
 *            a(9)=7895, a(10)=24074, a(11)=56043, a(12)=80117, a(13)=136160,
 *            a(14)=216277, a(15)=352437, a(16)=921151, a(17)=3115890
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 24, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* const array of the first 18 terms (a(0) … a(17)) */
    static const seq_t init_vals[18] = {
        (seq_t)1,   (seq_t)3,   (seq_t)7,   (seq_t)10,  (seq_t)17,  (seq_t)27,
        (seq_t)44,  (seq_t)115, (seq_t)389, (seq_t)7895, (seq_t)24074, (seq_t)56043,
        (seq_t)80117, (seq_t)136160, (seq_t)216277, (seq_t)352437,
        (seq_t)921151, (seq_t)3115890
    };

    /* Dynamically‑allocated cache, grows as needed */
    static seq_t *cache = NULL;          /* pointer to cached values      */
    static size_t cap = 0;               /* current allocated size       */
    static idx_t first_uncomputed = 18;  /* first index that is not yet cached */

    /* Initialise cache on first call */
    if (!cache) {
        /* Allocate a reasonable starting capacity (enough for n up to ~10k) */
        cap = 20001;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return zero */
        for (size_t i = 0; i < 18; ++i) cache[i] = init_vals[i];
    }

    /* Ensure the cache is large enough for index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1; /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap = new_cap;
        } else {
            /* If realloc fails, try a minimal incremental grow */
            new_cap = cap;
            while ((size_t)n >= new_cap) new_cap += 1024;
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* give up */
            cache = tmp;
            cap = new_cap;
        }
    }

    /* Return cached value if already computed */
    if (n < first_uncomputed) return cache[(size_t)n];

    /* Compute all missing terms up to n */
    for (idx_t i = first_uncomputed; i <= n; ++i) {
        seq_t val;
        if (i < 18)
            val = init_vals[(size_t)i];
        else
            val = (seq_t)8010 * cache[(size_t)(i - 9)] + cache[(size_t)(i - 18)];
        cache[(size_t)i] = val;
    }
    first_uncomputed = n + 1;
    return cache[(size_t)n];
}
