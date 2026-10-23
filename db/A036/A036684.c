/*
 * A036684  T(n+5,5) with T as in A036355.
 * https://oeis.org/A036684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036684
   Formula: a(n) = 3*A001872(n)+4*A001873(n)+A001874(n)
   This satisfies the linear recurrence
      a(n) = 6*a(n-1) + 30*a(n-4) + 6*a(n-5) + 30*a(n-8) + 10*a(n-9)
             – (9*a(n-2) + 10*a(n-3) + 41*a(n-6) + 6*a(n-7) + 9*a(n-10)
             + 6*a(n-11) + a(n-12))
   with seed values
      a(0)=8, a(1)=38, a(2)=149, a(3)=478, a(4)=1390, a(5)=3736,
      a(6)=9496, a(7)=23080, a(8)=54127, a(9)=123230, a(10)=273653,
      a(11)=594878.
   The implementation caches previously computed values for efficiency. */

#include <stdlib.h>

static const seq_t BASE[12] = {
    (seq_t)8,       (seq_t)38,      (seq_t)149,  (seq_t)478,
    (seq_t)1390,    (seq_t)3736,    (seq_t)9496,  (seq_t)23080,
    (seq_t)54127,   (seq_t)123230,  (seq_t)273653,(seq_t)594878
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    size_t up = (size_t)n;
    if (cache_cap < up + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < up + 1) new_cap = up + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_size <= up) {
        for (size_t i = cache_size; i <= up; ++i) {
            if (i < 12) {
                cache[i] = BASE[i];
            } else {
                const seq_t *c = cache;          /* for brevity */
                seq_t posSum =
                      6 * c[i-1]
                    + 30 * c[i-4]
                    + 6  * c[i-5]
                    + 30 * c[i-8]
                    + 10 * c[i-9];

                seq_t negSum =
                      9 * c[i-2]
                    + 10 * c[i-3]
                    + 41 * c[i-6]
                    + 6  * c[i-7]
                    + 9  * c[i-10]
                    + 6  * c[i-11]
                    +    c[i-12];

                cache[i] = posSum - negSum;
            }
        }
        cache_size = up + 1;
    }

    return cache[up];
}
