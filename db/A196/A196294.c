/*
 * A196294  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,1,4,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 2*a(n-1) + 4*a(n-2) + 6*a(n-3) - 9*a(n-4) - 38*a(n-5) - 32*a(n-6) - 11*a(n-7) + 6*a(n-8) + a(n-9)
   with initial values
   a(1)=3, a(2)=9, a(3)=20, a(4)=61, a(5)=187,
   a(6)=536, a(7)=1597, a(8)=4849, a(9)=14582.
*/

#include <stdlib.h>
#include <stdint.h>

/* External types provided by the build */
typedef uint64_t idx_t;   /* example definition – replace with the actual type */

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/* Implementation */
seq_t a(idx_t n)
{
    /* Pre‑computed initial values (1‑based index) */
    static const seq_t init[10] = {
        0,
        3,      /* a(1) */
        9,      /* a(2) */
        20,     /* a(3) */
        61,     /* a(4) */
        187,    /* a(5) */
        536,    /* a(6) */
        1597,   /* a(7) */
        4849,   /* a(8) */
        14582   /* a(9) */
    };

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* Grow cache if necessary */
    if (n > cached_n) {
        /* Size needed (1‑based, ignore cache[0]) */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – unlikely in test harness */
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i];
            } else {
                /* positive part */
                seq_t pos = 2 * cache[i - 1];
                pos += 4 * cache[i - 2];
                pos += 6 * cache[i - 3];
                pos += 6 * cache[i - 8];
                pos += cache[i - 9];
                /* negative part */
                seq_t neg = 9 * cache[i - 4];
                neg += 38 * cache[i - 5];
                neg += 32 * cache[i - 6];
                neg += 11 * cache[i - 7];
                /* result */
                cache[i] = pos - neg;   /* subtraction safe: recurrence guarantees non‑negative */
            }
        }
        cached_n = n;
    }

    return cache[n];
}
