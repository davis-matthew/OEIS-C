/*
 * A008127  Coordination sequence T1 for Zeolite Code LEV.
 * https://oeis.org/A008127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function of OEIS A008127. */

#include <stddef.h>
#include <stdlib.h>

/* Coefficients of the denominator polynomial D(x) of the generating function. */
static const int coeff[20] = {
    1,  /* c0 */
   -1,  /* c1 */
    1,  /* c2 */
    0,  /* c3 */
    0,  /* c4 */
   -2,  /* c5 */
    2,  /* c6 */
   -3,  /* c7 */
    1,  /* c8 */
   -1,  /* c9 */
    1,  /* c10 */
   -1,  /* c11 */
    3,  /* c12 */
   -2,  /* c13 */
    2,  /* c14 */
    0,  /* c15 */
    0,  /* c16 */
   -1,  /* c17 */
    1,  /* c18 */
   -1   /* c19 */
};

/* Coefficients of the numerator polynomial N(x) (degree 19). */
static const seq_t num_coeff[20] = {
    (seq_t)1,  /* n0 */
    (seq_t)3,  /* n1 */
    (seq_t)6,  /* n2 */
    (seq_t)12, /* n3 */
    (seq_t)22, /* n4 */
    (seq_t)34, /* n5 */
    (seq_t)46, /* n6 */
    (seq_t)57, /* n7 */
    (seq_t)66, /* n8 */
    (seq_t)71, /* n9 */
    (seq_t)71, /* n10 */
    (seq_t)66, /* n11 */
    (seq_t)57, /* n12 */
    (seq_t)46, /* n13 */
    (seq_t)34, /* n14 */
    (seq_t)22, /* n15 */
    (seq_t)12, /* n16 */
    (seq_t)6,  /* n17 */
    (seq_t)3,  /* n18 */
    (seq_t)1   /* n19 */
};

/* Simple memoization cache. */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;

/* Ensure the cache can hold indices up to `n`. */
static void ensure_capacity(size_t n)
{
    if (cache_len > n) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* Allocation failure is fatal in the test harness. */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the nth term of sequence A008127. */
seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];
    ensure_capacity((size_t)n);

    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        long long sum = 0;

        /* Sum_{k=1}^{19} coeff[k] * a_{i-k} for all valid indices. */
        size_t kmax = 19;
        if (i < 19) kmax = (size_t)i;
        for (size_t k = 1; k <= kmax; ++k)
        {
            sum += (long long)coeff[k] * (long long)cache[i - k];
        }

        long long val;
        if (i <= 19)                      /* use numerator coefficients for the first 20 terms */
            val = (long long)num_coeff[i] - sum;
        else                               /* homogeneous recurrence for higher terms */
            val = -sum;

        cache[i] = (seq_t)val;
    }
    return cache[n];
}
