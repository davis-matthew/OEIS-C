/*
 * A215291  Number of permutations of 0..floor((n*7-1)/2) on even squares of
 *          an nX7 array such that each row and column of even squares is
 *          increasing.
 * https://oeis.org/A215291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A060854(4,f3) * A060854(3,f4) * binomial(4*f3 + 3*f4,
 *            4*f3) where f3 = floor((n+1)/2), f4 = floor(n/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the hook‑length formula for
   A060854(a,k) which counts standard Young tableaux of an a×k rectangle.
   For a=3,4 the values are computed iteratively and cached. */

#include "../utils/binomial.h"

/* cache for A060854(4,k) and A060854(3,k) up to k = 5000 (enough for n ≤ 10000) */
static seq_t SYT4[5001] = {1};
static seq_t SYT3[5001] = {1};
static idx_t curK4 = 0;
static idx_t curK3 = 0;

/* compute or retrieve A060854(a,k) for a = 3 or 4 */
static seq_t get_SYT(int a, idx_t k)
{
    if (a == 4) {
        while (curK4 < k) {          /* extend cache up to k */
            ++curK4;
            seq_t prodN = 1;
            idx_t start = (idx_t)4 * (curK4 - 1) + 1;
            idx_t end   = (idx_t)4 * curK4;
            for (idx_t t = start; t <= end; ++t)
                prodN *= (seq_t)t;                     /* product of a consecutive integers */

            seq_t prodD = 1;
            for (idx_t m = 0; m < 4; ++m)
                prodD *= (seq_t)(curK4 + m);            /* product of (k+m) for m=0..3 */

            SYT4[curK4] = SYT4[curK4 - 1] * prodN / prodD;
        }
        return SYT4[k];
    } else { /* a == 3 */
        while (curK3 < k) {
            ++curK3;
            seq_t prodN = 1;
            idx_t start = (idx_t)3 * (curK3 - 1) + 1;
            idx_t end   = (idx_t)3 * curK3;
            for (idx_t t = start; t <= end; ++t)
                prodN *= (seq_t)t;

            seq_t prodD = 1;
            for (idx_t m = 0; m < 3; ++m)
                prodD *= (seq_t)(curK3 + m);

            SYT3[curK3] = SYT3[curK3 - 1] * prodN / prodD;
        }
        return SYT3[k];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; a(0) unused */

    idx_t f3 = (n + 1) / 2;              /* floor((n+1)/2) */
    idx_t f4 = n / 2;                    /* floor(n/2) */

    seq_t term4 = get_SYT(4, f3);        /* A060854(4,f3) */
    seq_t term3 = get_SYT(3, f4);        /* A060854(3,f4) */

    idx_t total = 4 * f3 + 3 * f4;
    idx_t choose = 4 * f3;               /* binomial(4*f3 + 3*f4, 4*f3) */

    seq_t bin = binomial(total, choose);

    return term4 * term3 * bin;
}
