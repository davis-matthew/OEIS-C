/*
 * A200061  Number of -n..n arrays x(0..6) of 7 elements with zero sum and
 *          elements alternately strictly increasing and strictly decreasing.
 * https://oeis.org/A200061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 2*a(n-1) -a(n-3) -a(n-5) +a(n-6) -2*a(n-7) +2*a(n-8) +a(n-9)
          -a(n-13) -2*a(n-14) +2*a(n-15) -a(n-16) +a(n-17) +a(n-19)
          -2*a(n-21) +a(n-22)
   Initial values (a(1)..a(22)):
   14, 472, 4224, 20978, 74338, 211242, 514168, 1115572, 2215290, 4099888,
   7165376, 11941380, 19119208, 29580842, 44432366, 65037696, 93057524,
   130487666, 179703518, 243503094, 325156994, 428455996 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑based) */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* initial 22 terms */
    static const seq_t init[23] = {
        0,
        14ULL, 472ULL, 4224ULL, 20978ULL, 74338ULL, 211242ULL, 514168ULL,
        1115572ULL, 2215290ULL, 4099888ULL, 7165376ULL, 11941380ULL,
        19119208ULL, 29580842ULL, 44432366ULL, 65037696ULL, 93057524ULL,
        130487666ULL, 179703518ULL, 243503094ULL, 325156994ULL, 428455996ULL
    };

    /* initialise cache with the first 22 terms */
    if (cap == 0) {
        cap = 22;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cap + 1));
        for (idx_t i = 1; i <= cap; ++i)
            cache[i] = init[i];
    }

    if (n <= cap)
        return cache[n];

    /* grow cache up to n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    for (idx_t i = cap + 1; i <= n; ++i) {
        seq_t pos = 0;
        /* positive contributions */
        pos += cache[i - 1];  pos += cache[i - 1];          /* 2*a(n-1) */
        pos += cache[i - 6];
        pos += cache[i - 8];  pos += cache[i - 8];          /* 2*a(n-8) */
        pos += cache[i - 9];
        pos += cache[i - 15]; pos += cache[i - 15];        /* 2*a(n-15) */
        pos += cache[i - 17];
        pos += cache[i - 19];
        pos += cache[i - 22];

        seq_t neg = 0;
        /* negative contributions (subtracted later) */
        neg += cache[i - 3];
        neg += cache[i - 5];
        neg += cache[i - 7];  neg += cache[i - 7];          /* 2*a(n-7) */
        neg += cache[i - 13];
        neg += cache[i - 14]; neg += cache[i - 14];        /* 2*a(n-14) */
        neg += cache[i - 16];
        neg += cache[i - 21]; neg += cache[i - 21];        /* 2*a(n-21) */

        /* final value: positive sum minus negative sum */
        cache[i] = pos - neg;
    }
    cap = n;
    return cache[n];
}
