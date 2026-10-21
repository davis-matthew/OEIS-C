/*
 * A234162  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234162
   Empirical recurrence: a(n) = 4*a(n-1) + 41*a(n-2) - 132*a(n-3) - 264*a(n-4)
   Initial values: a(1)=136, a(2)=766, a(3)=4296, a(4)=24158
   (for n≥5 the above recurrence holds) */

#include <stddef.h>
#include <stdbool.h>
/* No other headers needed; seq_t and idx_t are defined by the build */

static seq_t mul_small(seq_t x, unsigned int c)
{
    seq_t result = 0;
    seq_t term = x;
    while (c)
    {
        if (c & 1U) result += term;
        term += term;
        c >>= 1U;
    }
    return result;
}

/* maximum n we are willing to support; adjust if larger values are needed */
#define MAXN 20000

static seq_t cache[MAXN + 1];
static bool initialized = false;
static idx_t last_computed = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    if (!initialized)
    {
        /* set known initial values */
        cache[1] = (seq_t)136;
        cache[2] = (seq_t)766;
        cache[3] = (seq_t)4296;
        cache[4] = (seq_t)24158;
        last_computed = 4;
        initialized = true;
    }

    if (n > MAXN)
    {
        /* if n exceeds the pre‑allocated range, extend computation
           using only the last four values (no need to store the entire sequence) */
        /* Use a ring buffer for the last four terms */
        seq_t tmp[4];
        tmp[0] = cache[(last_computed >= 4) ? last_computed - 3 : 1];
        tmp[1] = cache[(last_computed >= 3) ? last_computed - 2 : 2];
        tmp[2] = cache[(last_computed >= 2) ? last_computed - 1 : 3];
        tmp[3] = cache[last_computed];

        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            seq_t val = mul_small(tmp[3], 4) + mul_small(tmp[2], 41)
                      - mul_small(tmp[1], 132) - mul_small(tmp[0], 264);
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            tmp[3] = val;
        }
        return tmp[3];
    }

    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            seq_t val = mul_small(cache[i - 1], 4) + mul_small(cache[i - 2], 41)
                      - mul_small(cache[i - 3], 132) - mul_small(cache[i - 4], 264);
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
