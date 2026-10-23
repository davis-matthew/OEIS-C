/*
 * A164049  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 496*a(n-6) +
 *            (n==6 ? 1 : 0), a(0)=1, a(1)=33, a(2)=1056, a(3)=33792,
 *            a(4)=1081344, a(5)=34603008, a(n>6) follows the homogeneous
 *            recurrence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocation size */
    static size_t len   = 0;   /* next index to compute (cache[0..len-1] filled) */

    /* ensure the cache has room for the first 6 terms and is initialized */
    if (cap < 7) {
        size_t new_cap = 7;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
        cache[0] = 1;
        cache[1] = 33;
        cache[2] = 1056;
        cache[3] = 33792;
        cache[4] = 1081344;
        cache[5] = 34603008;
        len   = 6;   /* cache[0..5] ready, next to compute is index 6 */
    }

    size_t idx = (size_t)n;

    /* enlarge allocation if necessary */
    if (idx >= cap) {
        size_t new_cap = idx + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute terms up to idx */
    for (size_t i = len; i <= idx; ++i) {
        seq_t val;
        if (i <= 5) {
            /* already known */
            val = cache[i];
        } else if (i == 6) {
            /* special forcing term of the generating function */
            seq_t sum = cache[5] + cache[4] + cache[3] + cache[2] + cache[1];
            val = 31 * sum;
            val -= 496 * cache[0];
            val += 1;  /* add the +1 from N(t) */
        } else {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            val = 31 * sum;
            val -= 496 * cache[i-6];
        }
        cache[i] = val;
    }
    len = idx + 1;
    return cache[idx];
}
