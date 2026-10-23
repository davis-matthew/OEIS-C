/*
 * A038740  a(n) = T(n,n-3), array T as in A038738.
 * https://oeis.org/A038740
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 9*a(n-1) - 34*a(n-2) + 71*a(n-3) - 90*a(n-4)
          + 71*a(n-5) - 34*a(n-6) + 9*a(n-7) - a(n-8)
   for n ≥ 11.
   Initial values used by the recurrence:
   a(3)=1, a(4)=9, a(5)=47, a(6)=188, a(7)=643,
   a(8)=1993, a(9)=5798, a(10)=16193.
   Offset is 3.                                     */
#include <stdlib.h>

static seq_t *cache  = NULL;  /* array of computed values   */
static idx_t   start  = 3;    /* first index stored in cache*/
static size_t  len    = 0;    /* number of stored values    */
static size_t  cap    = 0;    /* allocated capacity         */

static void
ensure_capacity(void)
{
    if (len == cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* behaviour for n < 3 is not relevant for the OEIS entry */
    if (n < 3) return 0;

    /* initialise cache with the first eight terms */
    if (len == 0) {
        const seq_t init[8] = {
            1ULL, 9ULL, 47ULL, 188ULL, 643ULL,
            1993ULL, 5798ULL, 16193ULL
        };
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
        len = 8;          /* cache[0] = a(3), ..., cache[7] = a(10) */
    }

    /* extend cache up to the requested index n */
    while (start + len <= n) {
        /* compute a(k) where k = start + len (current index) */
        seq_t a1 = cache[len - 1];
        seq_t a2 = cache[len - 2];
        seq_t a3 = cache[len - 3];
        seq_t a4 = cache[len - 4];
        seq_t a5 = cache[len - 5];
        seq_t a6 = cache[len - 6];
        seq_t a7 = cache[len - 7];
        seq_t a8 = cache[len - 8];

        seq_t next = 9 * a1
                     - 34 * a2
                     + 71 * a3
                     - 90 * a4
                     + 71 * a5
                     - 34 * a6
                     + 9  * a7
                     - a8;

        ensure_capacity();
        cache[len++] = next;
    }

    return cache[n - start];
}
