/*
 * A221448  Number of 3 X n binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 5*a(n-1) + 8*a(n-2)
 *            + 46*a(n-3) + 39*a(n-4) + 69*a(n-5) – 10*a(n-6) – 38*a(n-7) –
 *            68*a(n-8) – 40*a(n-9) + 50*a(n-10) + 18*a(n-11) – 13*a(n-12) –
 *            7*a(n-13) + 4*a(n-14) + 2*a(n-15) – a(n-16) + a(n-17) Initial
 *            values (offset 1): a(1)=1, a(2)=31, a(3)=221,...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* coefficients for the recurrence, index k=0 … 16 corresponds to a(n-k-1) */
static const int64_t coeff[17] = {
     5,  8, 46, 39, 69,
   -10,-38,-68,-40,50,
    18,-13,-7,  4, 2,
    -1,  1
};

/* initial 17 terms (offset 1) */
static const seq_t init[17] = {
    (seq_t)1,          (seq_t)31,
    (seq_t)221,        (seq_t)1465,
    (seq_t)10593,      (seq_t)76055,
    (seq_t)543081,     (seq_t)3883061,
    (seq_t)27769277,   (seq_t)198564543,
    (seq_t)1419847869, (seq_t)10152784401,
    (seq_t)72598518713,(seq_t)519122981715,
    (seq_t)3712041493221ULL,(seq_t)26543328598837ULL,
    (seq_t)189800757263741ULL
};

/* memoisation storage */
static seq_t *cache   = NULL;
static idx_t  cached  = 0;   /* highest n already computed   */
static idx_t  capacity = 0;  /* allocated entries (array size) */

seq_t a(idx_t n)
{
    /* handle first 17 terms directly */
    if (n <= 17ULL) {
        return init[n - 1];
    }

    /* ensure cache has space for the initial 17 values */
    if (capacity < 17ULL) {
        seq_t *tmp = (seq_t *)realloc(cache, 17ULL * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache   = tmp;
        capacity = 17ULL;
        for (idx_t i = 0ULL; i < 17ULL; ++i) {
            cache[i] = init[i];
        }
    }

    /* the cached index should be at least 17 now */
    if (cached < 17ULL) cached = 17ULL;

    /* grow cache if necessary for new n */
    if (capacity < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache   = tmp;
        capacity = n;
    }

    /* compute missing values from cached+1 up to n */
    for (idx_t i = cached + 1ULL; i <= n; ++i) {
        seq_t pos = 0;   /* sum of positive‑coefficient contributions   */
        seq_t neg = 0;   /* sum of negative‑coefficient contributions  */
        for (int k = 0; k < 17; ++k) {
            /* a(i-1-k) corresponds to cache index i-k-2 */
            seq_t t = cache[i - k - 2ULL];
            int64_t c = coeff[k];
            if (c >= 0) {
                pos += t * (seq_t)c;
            } else {
                neg += t * (seq_t)(-c);
            }
        }
        cache[i - 1ULL] = pos - neg;  /* pos >= neg for the sequence */
    }

    cached = n;
    return cache[n - 1];
}
