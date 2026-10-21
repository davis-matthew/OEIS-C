/*
 * A217451  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..2 n X 2 array.
 * https://oeis.org/A217451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 6*a(n-3) - 5*a(n-4) + a(n-5) -
 *            17*a(n-6) - 14*a(n-7) - 26*a(n-8) - 4*a(n-9) with a(1)=3,
 *            a(2)=15, a(3)=69, a(4)=253, a(5)=1049, a(6)=4205, a(7)=16887,
 *            a(8)=68015, a(9)=273471
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[0] is unused; indices start at 1 */
static idx_t cap   = 0;              /* current allocated capacity (>= needed) */
static const seq_t init_vals[10] = {0,         /* 0 unused */
    3,      /* a(1) */
    15,     /* a(2) */
    69,     /* a(3) */
    253,    /* a(4) */
    1049,   /* a(5) */
    4205,   /* a(6) */
    16887,  /* a(7) */
    68015,  /* a(8) */
    273471  /* a(9) */
};

static void ensure_capacity(idx_t need)
{
    if (need <= cap)
        return;

    idx_t old_cap = cap;
    idx_t new_cap = (cap ? cap << 1 : 16);
    while (new_cap < need)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap = new_cap;
    /* initialize new slots to zero */
    if (old_cap < new_cap)
        memset((char *)cache + old_cap * sizeof(seq_t),
               0,
               (new_cap - old_cap) * sizeof(seq_t));
}

static void compute_up_to(idx_t n)
{
    if (n == 0)
        return;

    ensure_capacity(n + 1);   /* we need indices 1..n */

    for (idx_t i = 1; i <= n; ++i)
    {
        if (i <= 9)
        {
            /* base values */
            cache[i] = init_vals[i];
        }
        else if (cache[i] == 0)   /* not computed yet */
        {
            seq_t pos = 3 * cache[i - 1]
                      + 3 * cache[i - 2]
                      + 6 * cache[i - 3];
            pos += cache[i - 5];

            seq_t neg = 5 * cache[i - 4]
                      + 17 * cache[i - 6]
                      + 14 * cache[i - 7]
                      + 26 * cache[i - 8]
                      + 4 * cache[i - 9];

            cache[i] = pos - neg;
        }
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return *(&cache[n]);   /* cache[n] may be zero for n==0 */
}
