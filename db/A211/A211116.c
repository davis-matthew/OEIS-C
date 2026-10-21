/*
 * A211116  Number of (n+1) X (n+1) -2..2 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one, two or three distinct values.
 * https://oeis.org/A211116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A211116 – recurrence:
        a(n) = 7*a(n‑1) – 15*a(n‑2) + a(n‑3) + 33*a(n‑4) – 28*a(n‑5) – 12*a(n‑6)
                 + 16*a(n‑7) + a(n‑8) – 2*a(n‑9)   for n ≥ 10
     with initial values
        a(1)=13, a(2)=31, a(3)=75, a(4)=177, a(5)=415, a(6)=963,
        a(7)=2227, a(8)=5137, a(9)=11855.
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms, 1‑based (stored 0‑based) */
static size_t cache_len = 0;     /* number of cached terms */

static const seq_t base_vals[9] = {
    (seq_t)13,    /* a(1) */
    (seq_t)31,    /* a(2) */
    (seq_t)75,    /* a(3) */
    (seq_t)177,   /* a(4) */
    (seq_t)415,   /* a(5) */
    (seq_t)963,   /* a(6) */
    (seq_t)2227,  /* a(7) */
    (seq_t)5137,  /* a(8) */
    (seq_t)11855  /* a(9) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1, 0 is undefined */

    /* enlarge cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure – terminate */
        cache = tmp;

        /* fill new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 9) {              /* base term a(i+1) */
                cache[i] = base_vals[i];
            } else {                  /* recurrence for a(i+1) */
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                seq_t a6 = cache[i-6];
                seq_t a7 = cache[i-7];
                seq_t a8 = cache[i-8];
                seq_t a9 = cache[i-9];

                /* 7*a1 -15*a2 + a3 +33*a4 -28*a5 -12*a6 +16*a7 + a8 -2*a9 */
                seq_t res = 7 * a1;
                res += a3;
                res += 33 * a4;
                res += 16 * a7;
                res += a8;
                res -= 15 * a2;
                res -= 28 * a5;
                res -= 12 * a6;
                res -= 2 * a9;

                cache[i] = res;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
