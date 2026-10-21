/*
 * A050927  Number of directed multigraphs with loops on 3 nodes with n arcs.
 * https://oeis.org/A050927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 70, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A050927
 *  Number of directed multigraphs with loops on 3 nodes with n arcs.
 *
 *  The generating function is
 *      (x^10 + 3x^8 + 10x^7 + 16x^6 + 12x^5 + 16x^4 + 10x^3 + 3x^2 + 1)
 *  --------------------------------------------------------------------------
 *  ----------------  (1-x^3)^3 (1-x^2)^4 (1-x)^2
 *
 *  From this we obtain the linear recurrence (order 19)
 *
 *      a(n) = rhs[n] - Σ_{k=1}^{min(n,19)} d[k] * a(n-k),
 *
 *  where rhs[n] is the coefficient of x^n in the numerator for n≤10
 *  (otherwise 0) and the recurrence coefficients d[0..19] are
 *
 *          1, -2, -3,  5,  8, -3, -19, -4, 24, 15,
 *          -15, -24,  4, 19,  3, -8, -5,  3,  2, -1.
 *
 *  The implementation below uses an on‑demand memoised computation
 *  and the provided helper mul_small() to avoid explicit overflow.
 *  The function works for any seq_t/idx_t defined by the build system.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* recurrence coefficients d[0..19] */
static const int d[20] = {
    1,   /* k = 0  */
   -2,  /* k = 1  */
   -3,  /* k = 2  */
    5,  /* k = 3  */
    8,  /* k = 4  */
   -3,  /* k = 5  */
   -19, /* k = 6  */
   -4,  /* k = 7  */
    24, /* k = 8  */
    15, /* k = 9  */
   -15, /* k =10  */
   -24, /* k =11  */
    4,  /* k =12  */
   19,  /* k =13  */
    3,  /* k =14  */
   -8,  /* k =15  */
   -5,  /* k =16  */
    3,  /* k =17  */
    2,  /* k =18  */
   -1   /* k =19  */
};

/* numerator coefficients rhs[n] for n = 0..10, zero for others */
static const seq_t rhs[11] = {
    (seq_t)1,  /* n = 0   */
    (seq_t)0,  /* n = 1   */
    (seq_t)3,  /* n = 2   */
    (seq_t)10, /* n = 3   */
    (seq_t)16, /* n = 4   */
    (seq_t)12, /* n = 5   */
    (seq_t)16, /* n = 6   */
    (seq_t)10, /* n = 7   */
    (seq_t)3,  /* n = 8   */
    (seq_t)0,  /* n = 9   */
    (seq_t)1   /* n =10   */
};

static seq_t *cache = NULL;      /* memoised sequence values */
static idx_t cache_len = 0;      /* number of values already computed */
static size_t cache_cap = 0;     /* allocated capacity */

/* Make sure the cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory */
    cache = tmp;
    cache_cap = new_cap;

    /* initialise new slots to zero (not strictly necessary) */
    for (size_t i = cache_len; i < cache_cap; ++i)
        cache[i] = (seq_t)0;
}

/* Main function requested by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* compute values up to n lazily */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val = 0;

        /* RHS coefficient (zero for i > 10) */
        if (i <= 10)
            val = rhs[(size_t)i];

        /* Accumulate contributions from previous terms */
        seq_t pos_sum = 0, neg_sum = 0;
        idx_t maxk = i < 19 ? (idx_t)i : 19;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            if (d[k] == 0)
                continue;
            seq_t tmp = mul_small(cache[i - k],
                                  (unsigned)((d[k] > 0) ? d[k] : -d[k]));
            if (d[k] > 0)
                pos_sum += tmp;
            else
                neg_sum += tmp;
        }

        val += neg_sum;
        /* final subtraction: guaranteed non‑negative */
        val -= pos_sum;  /* assume val >= pos_sum for all n */
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
