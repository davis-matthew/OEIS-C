/*
 * A234047  Number of (n+2)X(1+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally, diagonally downwards or antidiagonally
 *          downwards.
 * https://oeis.org/A234047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) -14*a(n-2) -53*a(n-3) +257*a(n-4) -8907*a(n-5)
 *            +3783*a(n-6) +24912*a(n-7) -11484*a(n-8) +1593*a(n-9)
 *            -5184*a(n-10) -3888*a(n-11) Initial values (a(1) .. a(11)):
 *            16488, 402000, 9780884, 237966121, 5789254059, 140841087777,
 *            3426377423999, 83356792653385, 2027901056098050, 493347038...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;          /* allocated size */
    static size_t used = 0;              /* number of values stored */
    static bool initialized = false;

    /* constants for initial 11 terms */
    static const seq_t init[11] = {
        (seq_t)16488ULL,
        (seq_t)402000ULL,
        (seq_t)9780884ULL,
        (seq_t)237966121ULL,
        (seq_t)5789254059ULL,
        (seq_t)140841087777ULL,
        (seq_t)3426377423999ULL,
        (seq_t)83356792653385ULL,
        (seq_t)2027901056098050ULL,
        (seq_t)49334703868796952ULL,
        (seq_t)1200212896541783312ULL
    };

    if (!initialized) {
        /* initialise cache with the first 11 terms */
        capacity = 11;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        used = 11;
        initialized = true;
    }

    if (n <= used)                 /* already computed */
        return cache[n-1];

    /* enlarge capacity if needed */
    if (n > capacity) {
        size_t newcap = capacity;
        if (newcap == 0) newcap = 1;
        while (newcap < (size_t)n)
            newcap <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = newcap;
    }

    /* compute terms from used+1 up to n */
    for (idx_t k = used + 1; k <= n; ++k) {
        seq_t val = (seq_t)25 * cache[k-2];
        val -= (seq_t)14 * cache[k-3];
        val -= (seq_t)53 * cache[k-4];
        val += (seq_t)257 * cache[k-5];
        val -= (seq_t)8907 * cache[k-6];
        val += (seq_t)3783 * cache[k-7];
        val += (seq_t)24912 * cache[k-8];
        val -= (seq_t)11484 * cache[k-9];
        val += (seq_t)1593 * cache[k-10];
        val -= (seq_t)5184 * cache[k-11];
        val -= (seq_t)3888 * cache[k-12];
        cache[k-1] = val;
    }

    used = n;
    return cache[n-1];
}
