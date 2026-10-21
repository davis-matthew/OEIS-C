/*
 * A221348  Number of nX3 arrays of occupancy after each element moves to some
 *          horizontal, diagonal or antidiagonal neighbor, without 2-loops.
 * https://oeis.org/A221348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) -162*a(n-2)
 *            +198*a(n-3)+305*a(n-4)-794*a(n-5)+668*a(n-6)-244*a(n-7)+72*a(n-8)
 *            Initial values:
 *            a(1)=0,a(2)=18,a(3)=370,a(4)=6893,a(5)=122690,a(6)=2152076,a(7)=37541550,a(8)=653557217
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t  computed_max = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    /* initialise cache with the first eight values */
    if (cache == NULL) {
        cap = 9;                         /* indices 0..8 (0 unused) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)0;             /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)370;
        cache[4] = (seq_t)6893;
        cache[5] = (seq_t)122690;
        cache[6] = (seq_t)2152076;
        cache[7] = (seq_t)37541550;
        cache[8] = (seq_t)653557217;
        computed_max = 8;
    }

    if (n <= computed_max)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        pos += 26 * cache[i-1];
        pos += 198 * cache[i-3];
        pos += 305 * cache[i-4];
        pos += 668 * cache[i-6];
        pos += 72  * cache[i-8];

        seq_t neg = 0;
        neg += 162 * cache[i-2];
        neg += 794 * cache[i-5];
        neg += 244 * cache[i-7];

        cache[i] = pos - neg;              /* safe: pos ≥ neg for all n ≥ 1 */
    }

    computed_max = n;
    return cache[n];
}
