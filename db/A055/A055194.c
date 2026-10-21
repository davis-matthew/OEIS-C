/*
 * A055194  Number of 3 X n binary matrices with distinct rows, up to row and
 *          column permutation; (n,3)-hypergraphs (including empty hyperedge
 *          but excluding multiple hyperedges).
 * https://oeis.org/A055194
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055194
 *  Number of 3×n binary matrices with distinct rows,
 *  up to row and column permutation.
 *
 *  The generating function is
 *    -x^2*(x^8-2*x^6-2*x^5+4*x^3-4*x-3)
 *    ----------------------------------------------------
 *    (x^3-1)^2*(x^2-1)^2*(x-1)^4
 *
 *  From this we obtain the linear recurrence of order 14:
 *
 *      a(n) = 4*a(n‑1) – 4*a(n‑2) – 2*a(n‑3) + 2*a(n‑4)
 *             + 4*a(n‑5) + 3*a(n‑6) – 12*a(n‑7) + 3*a(n‑8)
 *             + 4*a(n‑9) + 2*a(n‑10) – 2*a(n‑11) – 4*a(n‑12)
 *             + 4*a(n‑13) – a(n‑14)            + b(n)
 *
 *  where b(n) is zero except:
 *       b(2)=3,  b(3)=4,  b(5)=-4,  b(7)=2,  b(8)=2,  b(10)=-1.
 *
 *  We store the first 14 terms explicitly (a(0)=a(1)=0) and
 *  use the recurrence from n≥14 onward.  The implementation
 *  uses only unsigned arithmetic.  All operations are performed
 *  with seq_t, which is unsigned; the intermediate sums
 *  (`pos` and `neg`) are also seq_t, guaranteeing no
 *  signed overflow.  The method is O(n) per call and
 *  is memoised, making repeated calls inexpensive.
 */

#include <stdlib.h>   /* malloc, realloc */

static const int coef[15] = {
    1,   /* e0  */
   -4,   /* e1  */
    4,   /* e2  */
    2,   /* e3  */
   -2,   /* e4  */
   -4,   /* e5  */
   -3,   /* e6  */
   12,   /* e7  */
   -3,   /* e8  */
   -4,   /* e9  */
   -2,   /* e10 */
    2,   /* e11 */
    4,   /* e12 */
   -4,   /* e13 */
    1    /* e14 */
};

static const int rhs[11] = {
    0,   /* b0  */
    0,   /* b1  */
    3,   /* b2  */
    4,   /* b3  */
    0,   /* b4  */
   -4,   /* b5  */
    0,   /* b6  */
    2,   /* b7  */
    2,   /* b8  */
    0,   /* b9  */
   -1    /* b10 */
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of indices actually computed */
static size_t cache_cap = 0;   /* allocated capacity */

/* initialise the first 14 terms (a(0) … a(13)) */
static void init_base(void) {
    static const seq_t base[14] = {
        0,      /* a(0) */
        0,      /* a(1) */
        3,      /* a(2) */
        16,     /* a(3) */
        52,     /* a(4) */
        134,    /* a(5) */
        302,    /* a(6) */
        614,    /* a(7) */
        1159,   /* a(8) */
        2064,   /* a(9) */
        3504,   /* a(10) */
        5716,   /* a(11) */
        9018,   /* a(12) */
        13818   /* a(13) */
    };
    for (idx_t i = 0; i < 14; ++i)
        cache[i] = base[i];
    cache_len = 14;
}

/* grow the cache to at least size sz */
static void ensure_capacity(size_t sz) {
    if (cache_cap < sz) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < sz) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* values for n=0,1 are defined as 0 */

    /* lazily allocate cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* initialise base terms if not already done */
    if (cache_len < 14)
        init_base();

    /* compute only the missing entries */
    if (cache_len <= n) {
        ensure_capacity((size_t)(n + 1));

        for (idx_t i = cache_len; i <= n; ++i) {
            /* accumulate positive and negative contributions separately */
            seq_t pos = 0;
            seq_t neg = 0;

            /* contribution from the RHS b(n) */
            if (i <= 10 && rhs[i] != 0) {
                if (rhs[i] > 0)
                    pos += (seq_t)rhs[i];
                else
                    neg += (seq_t)(-rhs[i]);
            }

            /* contributions from previous terms */
            for (int k = 1; k <= 14; ++k) {
                if (i < (idx_t)k) continue;
                int c = coef[k];
                if (c == 0) continue;
                seq_t v = cache[i - k];
                if (c > 0)          /* positive coefficient → negative contribution */
                    neg += v * (seq_t)c;
                else                /* negative coefficient → positive contribution */
                    pos += v * (seq_t)(-c);
            }

            /* final value */
            cache[i] = pos - neg;   /* pos >= neg for all valid n */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
