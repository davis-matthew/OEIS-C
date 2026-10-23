/*
 * A186828  Diagonal sums of number triangle A186826.
 * https://oeis.org/A186828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*a(n) +(-7n+2)a(n-1) +(9n-7)a(n-2) +( -13n+36
 *            )a(n-3)
 *            +(3n-35)a(n-4)+(-5n+28)a(n-5)+(-5n+21)a(n-6)+(n-6)a(n-7)=0
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=14, a(4)=57, a(5)=247, a(6)=1124
 *            (Recurrence valid for n≥7)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The function depends on the compile‑time definition of seq_t and idx_t.
   For small integer types (uint8_t … uint64_t) the computation uses a
   signed __int128 accumulator.
   For arbitrary‑precision seq_t that overloads operators +,-,*,/ and can
   be constructed from signed integers, the same algorithm works because
   the arithmetic takes place in that type. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache        = NULL;
    static idx_t  capacity     = 0;      /* allocated length            */
    static idx_t  last_index   = 6;      /* highest index computed      */
    /* Initial values for n = 0 … 6. */
    static const seq_t init[7] = {
        (seq_t)1,  (seq_t)1,  (seq_t)4,  (seq_t)14,
        (seq_t)57, (seq_t)247, (seq_t)1124
    };

    /* First allocation. */
    if (cache == NULL) {
        capacity = 8;                       /* at least enough for init   */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allow graceful failure      */
        memcpy(cache, init, sizeof(init));
    }

    /* Grow cache if needed. */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* graceful failure           */
        cache   = tmp;
        capacity = new_cap;
    }

    /* Value already computed. */
    if (n <= last_index)
        return cache[n];

    /* Compute terms from last_index+1 up to n. */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* Signed 128‑bit accumulator for the weighted sum.  */
        __int128 sum = 0;

        /* Each term: coeff * a[i-k]  (k = 1 … 7).  */
        /* k = 1 : (-7i+2) * a[i-1] */
        sum += (__int128)(-7 * (int64_t)i + 2) * (__int128)cache[i - 1];
        /* k = 2 : (9i-7)  * a[i-2] */
        sum += (__int128)( 9 * (int64_t)i - 7) * (__int128)cache[i - 2];
        /* k = 3 : (-13i+36) * a[i-3] */
        sum += (__int128)(-13 * (int64_t)i + 36) * (__int128)cache[i - 3];
        /* k = 4 : (3i-35) * a[i-4] */
        sum += (__int128)( 3 * (int64_t)i - 35) * (__int128)cache[i - 4];
        /* k = 5 : (-5i+28) * a[i-5] */
        sum += (__int128)(-5 * (int64_t)i + 28) * (__int128)cache[i - 5];
        /* k = 6 : (-5i+21) * a[i-6] */
        sum += (__int128)(-5 * (int64_t)i + 21) * (__int128)cache[i - 6];
        /* k = 7 : (i-6) * a[i-7] */
        sum += (__int128)( 1 * (int64_t)i - 6) * (__int128)cache[i - 7];

        /* a(i) = -sum / (i+1)   (recurrence). */
        __int128 value = -sum / ( (__int128)i + 1 );

        /* Store the result. */
        cache[i] = (seq_t)value;
    }

    last_index = n;
    return cache[n];
}
