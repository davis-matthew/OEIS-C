/*
 * A116815  Number of permutations of length n which avoid the patterns 1234,
 *          1432, 3241.
 * https://oeis.org/A116815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-4) - 4*a(n-5) - a(n-6) +
 *            2*a(n-7) + 3*a(n-8) + a(n-9) (n ≥ 12) with special adjustments
 *            for n = 10, 11 based on the numerator of the rational generating
 *            function
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* WARNING:  the array `R` contains the R_n values
   (coefficients of the numerator).  For n > 11, R_n = 0.  */

#include <stdlib.h>

/* Initial terms a(1) … a(9).  Index 0 is unused. */
static const seq_t initial[10] = {
    0,      /* a(0) dummy */
    1,      /* a(1) */
    2,      /* a(2) */
    6,      /* a(3) */
    21,     /* a(4) */
    70,     /* a(5) */
    199,    /* a(6) */
    502,    /* a(7) */
    1232,   /* a(8) */
    2962    /* a(9) */
};

/* R_n for n = 1 … 11;  R[0] unused.  */
static const int R[12] = {
    0,     /* dummy 0 */
    -1,     /* R(1) */
    1,      /* R(2) */
    -1,     /* R(3) */
    -5,     /* R(4) */
    -15,    /* R(5) */
    -18,    /* R(6) */
    4,      /* R(7) */
    9,      /* R(8) */
    9,      /* R(9) */
    4,      /* R(10) */
    2       /* R(11) */
};

/* Minimal memoisation storage.  */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest index already computed (>=0) */

/* Compute a(n) and store needed intermediate values.  */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;                     /* already computed */

    /* Resize the cache */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* In a test environment we can abort; otherwise leave at 0. */
        abort();
    }
    cache = tmp;

    /* Compute all values from (cache_size + 1) up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = initial[i];
        } else {
            /* -------------
               Compute the recurrence term Summation = Σ d_k a[i-k]
               with the order:
                   +3*a[i-1]
                   +2*a[i-7]
                   +3*a[i-8]
                   +1*a[i-9]
               then subtract
                   -1*a[i-2]
                   -2*a[i-4]
                   -4*a[i-5]
                   -1*a[i-6]
               Finally apply the numerator adjustment R[i] (if any).
            ------------ */
            seq_t sum = 0;

            /* Positive contributions first (ensures no underflow) */
            sum += 3 * cache[i - 1];
            sum += 2 * cache[i - 7];
            sum += 3 * cache[i - 8];
            sum += cache[i - 9];

            /* Negative contributions */
            sum -= cache[i - 2];
            sum -= 2 * cache[i - 4];
            sum -= 4 * cache[i - 5];
            sum -= cache[i - 6];

            /* Numerator adjustment */
            int rn = (i <= 11) ? R[i] : 0;
            if (rn > 0) {
                sum -= (seq_t)rn;
            } else if (rn < 0) {
                sum += (seq_t)(-rn);
            }

            cache[i] = sum;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n <= 9)           /* base cases known */
        return initial[n];

    ensure_cache(n);  /* ensures cache[n] is computed */
    return cache[n];
}
