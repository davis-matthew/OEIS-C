/*
 * A189349  Number of nX3 array permutations with each element moving one
 *          space diagonally, horizontally or vertically.
 * https://oeis.org/A189349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 13, a(3) = 48, a(4) = 557, a(5) = 3632, a(6) =
 *            30769, a(7) = 229248, a(8) = 1815601, a(9) = 13960720, a(10) =
 *            108880333 For n > 10: a(n) = 2*a(n-1) + 41*a(n-2) + 44*a(n-3) -
 *            118*a(n-4) + 118*a(n-6) - 44*a(n-7) - 41*a(n-8) - 2*a(n-9) +
 *            a(n-10)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[10] = {
    (seq_t)0, (seq_t)13, (seq_t)48, (seq_t)557,
    (seq_t)3632, (seq_t)30769, (seq_t)229248,
    (seq_t)1815601, (seq_t)13960720, (seq_t)108880333
};

seq_t a(idx_t n)
{
    /* offset is 1, a(1) is the first term */
    if (n == 0)
        return (seq_t)0;               /* undefined, return 0 */

    static seq_t *buf = NULL;
    static size_t cap = 0;   /* allocated capacity (≥ len) */
    static size_t len = 0;   /* maximum n computed so far */

    /* initialise cache on first call */
    if (buf == NULL) {
        cap = 10 > n ? 10 : n;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) exit(1);           /* allocation failure */
        memcpy(buf, init, sizeof(init));
        len = 10;
    }

    /* enlarge the cache if needed */
    if (cap < (size_t)n) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 10;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        buf = tmp;
        cap = newcap;
    }

    /* compute missing terms */
    for (size_t k = len + 1; k <= (size_t)n; ++k) {
        if (k <= 10) {
            /* already initialised from init[10] */
        } else {
            /* indices are 1‑based for a(k), buf is 0‑based */
            size_t i = k - 1;          /* position of a(k) in buf */
            seq_t t1 =  2 * buf[i - 1];
            seq_t t2 = 41 * buf[i - 2];
            seq_t t3 = 44 * buf[i - 3];
            seq_t t4 =-118 * buf[i - 4];
            seq_t t5 = 118 * buf[i - 6];
            seq_t t6 =- 44 * buf[i - 7];
            seq_t t7 =- 41 * buf[i - 8];
            seq_t t8 =-  2 * buf[i - 9];
            seq_t t9 =    buf[i -10];
            buf[i] = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9;
        }
    }
    len = (size_t)n;

    return buf[n - 1];
}
