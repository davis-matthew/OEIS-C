/*
 * A232408  Number of (n+1) X (2+1) 0..2 arrays with every element both >= and
 *          <= some horizontal, diagonal or antidiagonal neighbor.
 * https://oeis.org/A232408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A232408: Number of (n+1)x3 0..2 arrays with each element bounded by
   a horizontal, diagonal or anti‑diagonal neighbour.
   Empirical linear recurrence of order 14:
        a(n) = 10 a(n-1) + 99 a(n-2) + 761 a(n-3) + 3010 a(n-4)
          + 3817 a(n-5) - 6407 a(n-6) - 73996 a(n-7) - 157764 a(n-8)
          - 156451 a(n-9) - 52584 a(n-10) + 63306 a(n-11) + 76836 a(n-12)
          + 4720 a(n-13) - 14400 a(n-14)
   (a(1) = 159, a(2) = 2689, …, a(15) = 66267244037598420899) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* ------------------------------------------------------------------ */
/* Recurrence data                                                        */
#define ORDER 14

/* coeff[i] multiplies a(n-i-1)  (i from 0 to ORDER-1) */
static const int coeff[ORDER] = {
    10,      /* a(n-1)  */
    99,      /* a(n-2)  */
    761,     /* a(n-3)  */
    3010,    /* a(n-4)  */
    3817,    /* a(n-5)  */
   -6407,    /* a(n-6)  */
  -73996,    /* a(n-7)  */
 -157764,    /* a(n-8)  */
 -156451,    /* a(n-9)  */
  -52584,    /* a(n-10) */
   63306,    /* a(n-11) */
   76836,    /* a(n-12) */
    4720,    /* a(n-13) */
  -14400     /* a(n-14) */
};

/* ------------------------------------------------------------------ */
/* Cache handling                                                        */
static seq_t *cache = NULL;      /* array of computed terms (1‑based) */
static idx_t cache_len = 0;      /* number of valid terms stored       */
static idx_t cache_cap = 0;      /* current allocation size (>=len)    */

/* multiply a seq_t by an integer coefficient (returns positive value) */
static inline seq_t mul_by_int(seq_t x, int k)
{
    /* k is always <= 0 or >0; we take absolute value for positive result */
    if (k >= 0)
        return x * (seq_t)k;
    return x * (seq_t)(-k);
}

/* ---------------------------------------------------------- initCache */
static void init_cache(void)
{
    /* Allocate space for the first 15 known terms */
    cache_cap = 15;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    /* Fill in the known terms using seq_from_str */
    cache[0]  = seq_from_str("159");
    cache[1]  = seq_from_str("2689");
    cache[2]  = seq_from_str("48971");
    cache[3]  = seq_from_str("890283");
    cache[4]  = seq_from_str("16262989");
    cache[5]  = seq_from_str("296516955");
    cache[6]  = seq_from_str("5408696699");
    cache[7]  = seq_from_str("98655002241");
    cache[8]  = seq_from_str("1799471776631");
    cache[9]  = seq_from_str("32822396381147");
    cache[10] = seq_from_str("598681879952041");
    cache[11] = seq_from_str("10919979187793811");
    cache[12] = seq_from_str("199180815849539195");
    cache[13] = seq_from_str("3633065333519893625");
    cache[14] = seq_from_str("66267244037598420899");
    cache_len = 15;
}

/* ------------------------------------------------------- recomputeTo */
static void recompute_to(idx_t n)
{
    /* Ensure cache has capacity for at least n terms */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – program must handle */
            return;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from (cache_len+1) up to n (1‑based indices) */
    for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
        /* idx is 1‑based, idx-1 is 0‑based for array */
        idx_t pos = idx - 1;
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        for (int k = 0; k < ORDER; ++k) {
            idx_t prev_idx = pos - (k + 1);          /* previous term index */
            seq_t prod = mul_by_int(cache[prev_idx], coeff[k]);
            if (coeff[k] >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        cache[pos] = sum_pos - sum_neg;
    }
    cache_len = n;
}

/* ------------------------------------------------------------------- a */
seq_t a(idx_t n)
{
    /* n must be >= 1 – caller guarantees this */

    if (!cache)            /* first call – initialise with explicit data */
        init_cache();

    if (n <= cache_len)     /* already known */
        return cache[n - 1];

    recompute_to(n);        /* extend cache to requested term */
    return cache[n - 1];
}
