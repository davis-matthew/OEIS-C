/*
 * A167899  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, …, a(15)=941192, a(16)=6588344. * For n≥17 a(n)
 *            = 6·(a(n-1)+a(n-2)+…+a(n-15)) – 21·a(n-16). * (Derived from the
 *            rational generating function in the OEIS entry.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t cache[8000] = {0};      /* big enough for n up to about 10000 */
static idx_t last_computed = (idx_t)-1;   /* no values yet */

/* first 17 values from the OEIS entry */
static const seq_t init_vals[17] = {
    1ULL,          /* a(0) */
    8ULL,           /* a(1) */
    56ULL,          /* a(2) */
    392ULL,         /* a(3) */
    2744ULL,        /* a(4) */
    19208ULL,       /* a(5) */
    134456ULL,      /* a(6) */
    941192ULL,      /* a(7) */
    6588344ULL,     /* a(8) */
    46118408ULL,    /* a(9) */
    322828856ULL,   /* a(10) */
    2259801992ULL,  /* a(11) */
    15818613944ULL, /* a(12) */
    110730297608ULL,/* a(13) */
    775112083256ULL,/* a(14) */
    5425784582792ULL,/* a(15) */
    37980492079516ULL/* a(16) */
};

seq_t a(idx_t n)
{
    /* Ensure cache contains all values up to n */

    /* Initialise first 17 entries on first call or when cache empty */
    if (last_computed == (idx_t)-1) {
        for (idx_t i = 0; i < 17; ++i)
            cache[i] = init_vals[i];
        last_computed = 16;
    }

    /* If requested n is already known, return it */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n, using the recurrence:
     *  a(i) = 6·(a(i-1)+…+a(i-15)) – 21·a(i-16)   for i ≥ 17
     */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* sum the last 15 terms */
        for (idx_t k = 1; k <= 15; ++k)
            sum += cache[i - k];
        sum = sum * (seq_t)6;
        sum -= cache[i - 16] * (seq_t)21;
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
