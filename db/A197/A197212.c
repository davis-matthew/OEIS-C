/*
 * A197212  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,3,2,1,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-3) + 10*a(n-4) + 16*a(n-5) + 8*a(n-6) +
 *            6*a(n-7) - 8*a(n-8) - 12*a(n-9) - 10*a(n-10) - 4*a(n-11) -
 *            a(n-12) Base values: a(1)=1, a(2)=5, a(3)=14, a(4)=44, a(5)=149,
 *            a(6)=479, a(7)=1537, a(8)=4971, a(9)=16088, a(10)=52020,
 *            a(11)=168111, a(12)=543379
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

static const seq_t init_12[12] = {
    1ULL, 5ULL, 14ULL, 44ULL, 149ULL, 479ULL,
    1537ULL, 4971ULL, 16088ULL, 52020ULL,
    168111ULL, 543379ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset, not used */

    static seq_t *cache   = NULL;   /* cached sequence values          */
    static idx_t  cached  = 0;      /* number of values cached         */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure – unlikely */
        for (idx_t i = 0; i < 12; ++i) cache[i] = init_12[i];
        cached = 12;
    }

    /* if the requested term is already cached */
    if (n <= cached) return cache[n - 1];

    /* extend cache to size n */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) return 0;               /* allocation failure – unlikely */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cached; i < n; ++i) {
        /* i is 0‑based index of a(i+1); i >= 12 here */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        sum_pos += (seq_t)2 * cache[i - 1];   /* a(n-1) */
        sum_pos += (seq_t)8 * cache[i - 3];   /* a(n-3) */
        sum_pos += (seq_t)10 * cache[i - 4];  /* a(n-4) */
        sum_pos += (seq_t)16 * cache[i - 5];  /* a(n-5) */
        sum_pos += (seq_t)8 * cache[i - 6];   /* a(n-6) */
        sum_pos += (seq_t)6 * cache[i - 7];   /* a(n-7) */

        sum_neg += (seq_t)8 * cache[i - 8];   /* a(n-8) */
        sum_neg += (seq_t)12 * cache[i - 9];  /* a(n-9) */
        sum_neg += (seq_t)10 * cache[i - 10]; /* a(n-10) */
        sum_neg += (seq_t)4 * cache[i - 11];  /* a(n-11) */
        sum_neg += cache[i - 12];             /* a(n-12) */

        cache[i] = sum_pos - sum_neg;         /* final value */
    }

    cached = n;          /* now we have terms up to a(n) */
    return cache[n - 1];
}
