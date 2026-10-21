/*
 * A229659  9-section a(9n+k) gives k-th differences of a for k=0..8 with
 *          a(n)=0 for n<8 and a(8)=1.
 * https://oeis.org/A229659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, eigen
 * Formula:   not stated
 * Range:     i8 n <= 106, i16 n <= 1006, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(9*n + k) = Σ_{j=0..k} (-1)^j * C(k,j) * a(n + k – j),
   with a(n)=0 for n<8 and a(8)=1. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Binomial coefficients for k up to 8.  The table is small; values are
   fits in unsigned int. */
static const unsigned int binomC[9][9] = {
    {1,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0},
    {1,3,3,1,0,0,0,0,0},
    {1,4,6,4,1,0,0,0,0},
    {1,5,10,10,5,1,0,0,0},
    {1,6,15,20,15,6,1,0,0},
    {1,7,21,35,35,21,7,1,0},
    {1,8,28,56,70,56,28,8,1}
};

/* Cached sequence values. */
static seq_t *cache      = NULL;
static size_t cap         = 0;
static idx_t  last_computed = 8;   /* last index that has a defined value */

/* Ensure internal buffer can hold index n.  Grow by at least a factor of
   two whenever needed.  The first allocation also seeds the base values
   a(0..7)=0 and a(8)=1. */
static void ensure_capacity(idx_t n)
{
    if (cap == 0) {
        /* First allocation: cover at least up to n.  Allocate a bit more
           to avoid frequent reallocations. */
        size_t start = (size_t)n + 1;
        if (start < 9) start = 9;
        cache = (seq_t *)malloc(start * sizeof(seq_t));
        if (cache == NULL) return;   /* should never happen in small tests */
        cap   = start;
        /* Seed base values */
        for (idx_t i = 0; i < 8; ++i) cache[(size_t)i] = 0;
        cache[(size_t)8] = 1;
    } else if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;
    if (n == 8) return 1;

    if (n <= last_computed) return cache[(size_t)n];

    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t q = i / 9;
        idx_t k = i % 9;
        seq_t sum = 0;

        for (idx_t j = 0; j <= k; ++j) {
            seq_t term   = cache[(size_t)(q + (k - j))];
            unsigned int coeff = binomC[(size_t)k][(size_t)j];
            if (j & 1)
                sum -= term * (seq_t)coeff;
            else
                sum += term * (seq_t)coeff;
        }
        cache[(size_t)i] = sum;
    }
    last_computed = n;
    return cache[(size_t)n];
}
