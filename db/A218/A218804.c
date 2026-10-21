/*
 * A218804  Hilltop maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or antidiagonal neighbor in a random 0..3 n X 2 array.
 * https://oeis.org/A218804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A218804
   Recurrence:
     a(1)=3, a(2)=15, a(3)=57, a(4)=225, a(5)=895, a(6)=3551, a(7)=14089,
     a(n)=3*a(n-1)+3*a(n-2)+3*a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)   (n>=8) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..7 */
    static const seq_t base[7] = {3, 15, 57, 225, 895, 3551, 14089};

    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;    /* largest n for which cache[n‑1] is valid */

    if (n == 0) return 0;     /* undefined for n=0 – return 0 for safety */

    if (n <= 7) return base[n - 1];

    /* Lazy initialization of cache with the base values */
    if (cached == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 7);
        if (!cache) return 0; /* allocation failure */
        for (idx_t i = 0; i < 7; ++i) cache[i] = base[i];
        cached = 7;
    }

    /* Grow cache if needed */
    if (n > cached) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;

        /* Compute new terms from cached+1 up to n */
        for (idx_t k = cached + 1; k <= n; ++k) {
            idx_t i = k - 1;   /* array index for a(k) */
            seq_t r =
                (seq_t)3 * cache[i - 1] +
                (seq_t)3 * cache[i - 2] +
                (seq_t)3 * cache[i - 3] +
                cache[i - 4] + cache[i - 5] + cache[i - 6] + cache[i - 7];
            cache[i] = r;
        }
        cached = n;
    }

    return cache[n - 1];
}
