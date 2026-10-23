/*
 * A025873  Expansion of 1/((1-x^4)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A025873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025873 – Number of partitions of n into parts 4, 8 and 11
   Formula: a(n) = a(n‑4) + a(n‑8) + a(n‑11) – a(n‑12) – a(n‑15) – a(n‑19) + a(n‑23)
   Initial values:
     a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=0,
     a(8)=2, a(9)=0, a(10)=0, a(11)=1, a(12)=2, a(13)=0, a(14)=0,
     a(15)=1, a(16)=3, a(17)=0, a(18)=0, a(19)=2, a(20)=3, a(21)=0,
     a(22)=1, a(23)=2
*/
#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[24] = {
    1, 0, 0, 0, 1, 0, 0, 0,
    2, 0, 0, 1, 2, 0, 0, 1,
    3, 0, 0, 2, 3, 0, 1, 2
};

seq_t a(idx_t n)
{
    /* small indices – return the known constants directly   */
    if (n < 24)
        return init_vals[n];

    /* static memoisation storage                 */
    static seq_t *cache = NULL;      /* array of computed a(i)   */
    static size_t cap   = 0;         /* allocated size of cache  */
    static idx_t  computed = 0;      /* highest index that has a value   */

    /* First use – initialise cache if necessary */
    if (cache == NULL) {
        /* Allocate at least enough space for indices up to n */
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 24)
            init_cap = 24;   /* ensure space for the 24 initial values */

        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failed – return 0 as a safe fallback */

        /* copy the initial 24 values into the cache   */
        memcpy(cache, init_vals, sizeof(init_vals));

        cap = init_cap;
        computed = 23;
    }
    /* Expand the cache if the requested index is beyond the current capacity */
    else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failed – return 0 as a safe fallback */

        /* zero the newly allocated part   */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));

        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 4)  val += cache[i - 4];
            if (i >= 8)  val += cache[i - 8];
            if (i >= 11) val += cache[i - 11];
            if (i >= 12) val -= cache[i - 12];
            if (i >= 15) val -= cache[i - 15];
            if (i >= 19) val -= cache[i - 19];
            if (i >= 23) val += cache[i - 23];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
