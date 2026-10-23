/*
 * A121485  Number of nondecreasing Dyck paths of semilength n and having no
 *          peaks at even level (n>=0). A nondecreasing Dyck path is a Dyck
 *          path for which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)-2*a(n-3)-4*a(n-4)+a(n-6) for n>6; a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=4, a(5)=8, a(6)=16
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* no external dependencies – only static cache */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n<1 */

    /* static cache state */
    static idx_t  cache_cap   = 0;       /* allocated size in elements */
    static idx_t  computed_n  = 0;       /* highest fully computed index */
    static seq_t *cache       = NULL;    /* array: cache[i] holds a(i) */

    /* ensure enough space for index n (index 0 is unused) */
    if (n >= cache_cap) {
        idx_t newcap = n + 1;            /* we need indices 0..n */
        if (newcap < 2)
            newcap = 2;                  /* at least two elements */
        /* double the size until it fits */
        while (newcap <= cache_cap)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)                         /* allocation failed */
            return 0;
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing values up to n */
    if (computed_n < n) {
        /* initialize base case at index 0 to avoid garbage */
        if (cache[0] == 0) cache[0] = 0;

        for (idx_t i = computed_n + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = 1;
            else if (i == 2)
                cache[i] = 1;
            else if (i == 3)
                cache[i] = 2;
            else if (i == 4)
                cache[i] = 4;
            else if (i == 5)
                cache[i] = 8;
            else if (i == 6)
                cache[i] = 16;
            else {
                /* recurrence valid for i>=7 */
                seq_t t1 = 4 * cache[i - 2];
                seq_t t2 = 2 * cache[i - 3];
                seq_t t3 = 4 * cache[i - 4];
                seq_t t4 = cache[i - 6];
                cache[i] = cache[i - 1] + t1 - t2 - t3 + t4;
            }
        }
        computed_n = n;
    }

    return cache[n];
}
