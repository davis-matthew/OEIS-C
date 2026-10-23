/*
 * A008248  Coordination sequence T2 for Zeolite Code YUG.
 * https://oeis.org/A008248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *
 * a(n) = 2*a(n-3) +   a(n-5) - a(n-6) + a(n-7) - 2*a(n-8)
 *        - 2*a(n-10)+ a(n-11)- a(n-12) + a(n-13) + 2*a(n-15)- a(n-18)
 *        + b(n)
 *
 * where b(n) are the coefficients of the numerator
 * 1 + 4x +10x² +20x³ +31x⁴ +40x⁵ +42x⁶ +33x⁷ +17x⁸ +x⁹ -17x¹⁰ -35x¹¹
 * -42x¹² -38x¹³ -32x¹⁴ -24x¹⁵ -11x¹⁶ +0x¹⁷ +3x¹⁸ +0x¹⁹ -2x²⁰ -x²¹
 *
 * Initial values:
 * a(0)=1, a(1)=4, a(2)=10, a(3)=22, a(4)=39, a(5)=61, a(6)=89, a(7)=118,
 * a(8)=153, a(9)=198, a(10)=241, a(11)=287, a(12)=349, a(13)=409, a(14)=465,
 * a(15)=541, a(16)=619, a(17)=688, a(18)=777, a(19)=871, a(20)=954,
 * a(21)=1056, a(22)=1165, a(23)=1264, …
 */

#include <stdlib.h>

/* offsets (k) of recurrence coefficients (a(n-k)) */
static const idx_t OFFSETS[] = {3,5,6,7,8,10,11,12,13,15,18};
/* corresponding integer coefficients (cf) */
static const int COEF[]    = {2,1,-1,1,-2,-2,1,-1,1,2,-1};

/* numerator coefficients b(n) for 0 ≤ n ≤ 21 (0 beyond). */
static const int NUM[22] = {
    1,  4, 10, 20, 31, 40, 42, 33, 17,  1,
   -17,-35,-42,-38,-32,-24,-11,  0,  3,  0,
   -2,-1
};

static seq_t *cache   = NULL; /* cache of computed values */
static size_t cache_len = 0;   /* length of the cache (number of values stored) */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* Compute the n-th term of OEIS sequence A008248 */
seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (cache == NULL) {
        cache_cap = 64;
        if ((size_t)n + 1 > cache_cap)
            cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > cache_cap) {
        while (cache_cap < ((size_t)n + 1))
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    if ((size_t)n >= cache_len) {
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            seq_t pos = 0, neg = 0;          /* separate sums for positive / negative contributions */

            /* Numerator contribution b(idx) */
            if (idx <= 21) {
                int c = NUM[idx];
                if (c > 0)
                    pos += (seq_t)c;
                else
                    neg += (seq_t)(-c);
            }

            /* Recurrence contributions */
            for (size_t j = 0; j < sizeof(OFFSETS)/sizeof(OFFSETS[0]); ++j) {
                idx_t off = OFFSETS[j];
                if ((idx_t)idx < off)   /* term not defined */
                    continue;
                seq_t term = cache[(size_t)idx - off];
                int   cf   = COEF[j];
                if (cf > 0)
                    pos += term * (seq_t)cf;     /* coefficient > 0 */
                else
                    neg += term * (seq_t)(-cf);  /* coefficient < 0 */
            }

            /* Final value: pos - neg (pos ≥ neg for all n) */
            cache[(size_t)idx] = pos - neg;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
