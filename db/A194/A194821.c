/*
 * A194821  a(n) = 1+floor(Sum_{k=1..n} <((-1)^k)*k*sqrt(2)>), where < > =
 *          fractional part.
 * https://oeis.org/A194821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1+floor(Sum_{k=1..n} <((-1)^k)*k*sqrt(2)>)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int128_floor_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/int128_floor_div.h"

/* ---------- internal helpers ---------- */

/* integer square root for unsigned __int128
   returns floor(sqrt(x))                     */
static unsigned __int128 int128_sqrt(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---------- global static data ---------- */

static unsigned __int128 SCALE = ((unsigned __int128)1) << 62;   /* 2^62 */
static unsigned __int128 S = 0;                                 /* floor(sqrt(2)*SCALE) */
static int initialized = 0;

/* cumulative sums of (-1)^k * fractional part of k*sqrt(2)
   cum[0] = 0, cum[n] = Σ_{k=1..n} (-1)^k * {k*sqrt(2)} * SCALE   */
static signed __int128 *cum = NULL;
static size_t cum_cap = 0;   /* number of elements currently allocated */

/* initialise S and the cumulative array for n = 0 */
static void init_seq(void)
{
    if (!initialized) {
        unsigned __int128 tmp = ((unsigned __int128)2) * SCALE * SCALE;
        S = int128_sqrt(tmp);          /* S = floor(sqrt(2)*SCALE) */

        cum = (signed __int128 *)malloc(sizeof(signed __int128));
        cum[0] = 0;
        cum_cap = 1;
        initialized = 1;
    }
}

/* ---- main function to be called ---- */

seq_t a(idx_t n)
{
    init_seq();

    /* extend cumulative array if needed */
    if ((size_t)n + 1 > cum_cap) {
        size_t old_cap = cum_cap;
        size_t new_cap = (size_t)n + 1;
        cum = (signed __int128 *)realloc(cum, new_cap * sizeof(signed __int128));
        signed __int128 sum = cum[old_cap - 1];        /* current cumulative sum */

        for (idx_t k = old_cap; k <= n; ++k) {
            /* compute fractional part numerator: f_k = (k*S) mod SCALE */
            unsigned __int128 prod = (unsigned __int128)k * S;
            unsigned __int128 f_k = prod % SCALE;

            /* update sum: (-1)^k sign */
            if ((k & 1) == 0)   /* k even -> + */
                sum += (signed __int128)f_k;
            else                /* k odd  -> - */
                sum -= (signed __int128)f_k;

            cum[k] = sum;
        }
        cum_cap = new_cap;
    }

    signed __int128 sum_f = cum[n];
    signed __int128 floor_div = int128_floor_div(sum_f, SCALE);
    signed __int128 result = floor_div + 1;

    return (seq_t) result;
}
