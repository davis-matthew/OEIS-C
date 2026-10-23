/*
 * A037705  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + cycle[(n-1)%4], a(1)=1; cycle = {1,3,0,2} (the
 *            repeating base‑7 digits)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static size_t cache_cap = 0;  /* number of terms currently cached */

/* Return the n-th term of OEIS A037705 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* For any n < 1 we return 0 safely. */
    if (n == 0) return (seq_t)0;

    size_t need = (size_t)n;                 /* maximal index requested */
    if (need > cache_cap) {                  /* need to grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        /* Compute new terms, filling from cache_cap+1 up to need */
        /* cycle[0]=1, cycle[1]=3, cycle[2]=0, cycle[3]=2 */
        static const unsigned char cycle[4] = {1, 3, 0, 2};

        /* First term */
        if (cache_cap == 0) {
            cache[0] = (seq_t)1;
        }

        for (size_t i = cache_cap + 1; i <= need; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;          /* already handled, but keep for safety */
            } else {
                /* cache[i-1] = cache[i-2]*7 + cycle[(i-1)%4] */
                seq_t mult = cache[i - 2] * (seq_t)7;
                seq_t add  = (seq_t)cycle[(i - 1) % 4];
                cache[i - 1] = mult + add;
            }
        }
        cache_cap = need;
    }
    return cache[n - 1];
}
