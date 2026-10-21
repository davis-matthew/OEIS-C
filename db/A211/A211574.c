/*
 * A211574  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (empirical recurrence of order 11):
 *  a(1)=24,   a(2)=76,   a(3)=192,   a(4)=456,   a(5)=1046,
 *  a(6)=2408, a(7)=5458, a(8)=12624, a(9)=28762, a(10)=67278,
 *  a(11)=154580,
 *  a(n) = 6*a(n-1) - 3*a(n-2) - 46*a(n-3) + 88*a(n-4) + 49*a(n-5)
 *       - 231*a(n-6) + 82*a(n-7) + 168*a(n-8) - 100*a(n-9)
 *       - 36*a(n-10) + 24*a( n-11)   for n ≥ 12
 */
#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t init[12] = {
    0ULL,        /* a(0) – unused, dummy */
    24ULL,       /* a(1) */
    76ULL,       /* a(2) */
    192ULL,      /* a(3) */
    456ULL,      /* a(4) */
    1046ULL,     /* a(5) */
    2408ULL,     /* a(6) */
    5458ULL,     /* a(7) */
    12624ULL,    /* a(8) */
    28762ULL,    /* a(9) */
    67278ULL,    /* a(10) */
    154580ULL    /* a(11) */
};

seq_t a(idx_t n)
{
    /* o.k. to return 0 for indices before the offset */
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cache_sz = 0;   /* number of elements actually stored */

    size_t N = (size_t)n;          /* size_t index for array access */

    /* Resize cache if necessary */
    if (N + 1 > cache_sz) {
        size_t new_sz = N + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();

        /* If we previously had a smaller cache, fill new slots with initial values */
        if (cache_sz < 12) {
            size_t start = cache_sz;
            if (start == 0) start = 1;          /* skip dummy a(0) */
            for (size_t i = start; i <= N && i < 12; ++i) {
                tmp[i] = init[i];
            }
        }

        /* Compute new terms (those not covered by the initial table) */
        if (new_sz > 12) {
            size_t start = cache_sz;
            if (start < 12) start = 12;
            for (size_t i = start; i <= N; ++i) {
                seq_t t1 = 6 * tmp[i-1];
                seq_t t2 = 3 * tmp[i-2];
                seq_t t3 = 46 * tmp[i-3];
                seq_t t4 = 88 * tmp[i-4];
                seq_t t5 = 49 * tmp[i-5];
                seq_t t6 = 231 * tmp[i-6];
                seq_t t7 = 82 * tmp[i-7];
                seq_t t8 = 168 * tmp[i-8];
                seq_t t9 = 100 * tmp[i-9];
                seq_t t10 = 36 * tmp[i-10];
                seq_t t11 = 24 * tmp[i-11];

                tmp[i] = t1 - t2 - t3 + t4 + t5 - t6 + t7 + t8 - t9 - t10 + t11;
            }
        }

        cache = tmp;
        cache_sz = new_sz;
    }

    return cache[N];
}
