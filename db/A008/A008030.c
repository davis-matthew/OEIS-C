/*
 * A008030  Coordination sequence T2 for Zeolite Code AFY.
 * https://oeis.org/A008030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 211, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence (coefficients from the denominator of the generating function):
   a(0)=1, a(1)=4, a(2)=9, a(3)=16, a(4)=23, a(5)=34, a(6)=57, a(7)=82,
   a(8)=98, a(9)=115, a(10)=141, a(11)=178, a(12)=224, a(13)=259, a(14)=282,
   a(15)=322.
   For n ≥ 16:
   a(n) = -(-a(n-1) + 2*a(n-2) - 3*a(n-3) + 3*a(n-4) - 5*a(n-5) +
           4*a(n-6) - 5*a(n-7) + 5*a(n-8) - 4*a(n-9) + 5*a(n-10) -
           3*a(n-11) + 3*a(n-12) - 2*a(n-13) + a(n-14) - a(n-15));
*/

static const seq_t FIRST[16] = {
    1, 4, 9, 16, 23, 34, 57, 82,
    98,115,141,178,224,259,282,322
};

static const int COEFF[15] = {
    -1, 2, -3, 3, -5, 4, -5, 5, -4, 5,
    -3, 3, -2, 1, -1
};

static seq_t *cache   = NULL;
static idx_t   clen   = 0;
static idx_t   cap    = 0;

static void grow_cache(idx_t n)
{
    if (cache && clen > n) return;

    idx_t want = n + 1;
    if (!cache) {
        cap = 32;
        if (cap < want) cap = want;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (int i = 0; i < 16; ++i) cache[i] = FIRST[i];
        clen = 16;
    } else if (cap < want) {
        while (cap < want) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    for (idx_t i = clen; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 15; ++k) {
            seq_t v = cache[i - 1 - k];
            int c   = COEFF[k];
            if (c >= 0)
                pos += v * (seq_t)c;
            else
                neg += v * (seq_t)(-c);
        }
        cache[i] = neg - pos;
    }
    clen = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 16) return FIRST[n];
    grow_cache(n);
    return cache[n];
}
