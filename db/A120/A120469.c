/*
 * A120469  Expansion of -(20*x^12 + 184*x^11 + 121*x^10 - 915*x^9 -1 524*x^8
 *          - 132*x^7 + 1068*x^6 + 581*x^5 - 71*x^4 - 112*x^3 - 14*x^2 + 5*x
 *          +1) / ((3*x +1) * (16*x^12 + 140*x^11 + 2*x^10 - 968*x^9 - 830*x^8
 *          + 946*x^7 + 982*x^6 - 239*x^5 - 351*x^4 - 5*x^3 + 39*x^2 + 3*x
 *          -1)).
 * https://oeis.org/A120469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function of
   A120469:
       D(x) * A(x) = N(x)
   where
       D(x) = (3x+1) * (16x^12 + 140x^11 + 2x^10 - 968x^9 - 830x^8 + 946x^7 +
                         982x^6 - 239x^5 - 351x^4 - 5x^3 + 39x^2 + 3x - 1)
   is a degree‑13 polynomial

       d0 = -1, d1 = 0, d2 = 48, d3 = 112, d4 = -366, d5 = -1292,
       d6 = 265, d7 = 3892, d8 = 2008, d9 = -3458, d10 = -2902,
       d11 = 146, d12 = 436, d13 = 48

   and
       N(x) = -(20x^12 + 184x^11 + 121x^10 - 915x^9 - 1524x^8 - 132x^7 +
                1068x^6 + 581x^5 - 71x^4 - 112x^3 - 14x^2 + 5x + 1)

       b0 = -1, b1 = -5, b2 = 14, b3 = 112, b4 = 71, b5 = -581,
       b6 = -1068, b7 = 132, b8 = 1524, b9 = 915, b10 = -121,
       b11 = -184, b12 = -20

   For n ≥ 13 the right‑hand side is zero, giving the homogeneous
   recurrence
       a(n) = 48·a(n-2) + 112·a(n-3) − 366·a(n-4) − 1292·a(n-5)
              + 265·a(n-6) + 3892·a(n-7) + 2008·a(n-8)
              − 3458·a(n-9) − 2902·a(n-10) + 146·a(n-11)
              + 436·a(n-12) + 48·a(n-13)

   Initial values (obtained by the full non‑homogeneous recurrence)
       a(0)=1, a(1)=5, a(2)=34, a(3)=240, a(4)=1755, a(5)=12787,
       a(6)=93549, a(7)=683653, a(8)=4999040, a(9)=36546925,
       a(10)=267210674, a(11)=1953631757, a(12)=14283593763.
*/
#include <stdlib.h>

static const int D[14] = {
    -1,   /* d0 */
     0,   /* d1 */
     48,  /* d2 */
    112,  /* d3 */
   -366,  /* d4 */
  -1292,  /* d5 */
    265,  /* d6 */
   3892,  /* d7 */
    2008, /* d8 */
   -3458, /* d9 */
   -2902, /* d10 */
    146,  /* d11 */
    436,  /* d12 */
     48   /* d13 */
};

static const int B[13] = {
    -1,   /* b0 */
    -5,   /* b1 */
    14,   /* b2 */
   112,   /* b3 */
    71,   /* b4 */
  -581,   /* b5 */
 -1068,   /* b6 */
   132,   /* b7 */
  1524,   /* b8 */
   915,   /* b9 */
  -121,   /* b10 */
  -184,   /* b11 */
  -20     /* b12 */
};

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Lazy allocation */
    if (!cache) {
        cache_cap = 256;                 /* arbitrary start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 13; ++k) {
            int c = D[k];
            if (c == 0) continue;
            if ((idx_t)k > i) continue;   /* term a(i-k) is zero for negative index */
            seq_t v = cache[(size_t)(i - k)];
            if (c > 0)
                pos += (seq_t)c * v;
            else
                neg += (seq_t)(-c) * v;
        }

        seq_t val = pos - neg;          /* sum_{k>=1} d_k * a(i-k) */

        if (i <= 12) {                  /* non‑homogeneous part */
            int b = B[(int)i];
            if (b >= 0)
                val -= (seq_t)b;
            else
                val += (seq_t)(-b);
        }

        cache[(size_t)i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
