/*
 * A200892  Number of 0..n arrays x(0..8) of 9 elements without any interior
 *          element greater than both neighbors.
 * https://oeis.org/A200892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 200 a(2) = 4059 a(3) = 34350 a(4) = 181336 a(5) = 710976
 *            a(6) = 2269938 a(7) = 6233356 a(8) = 15250675 a(9) = 34054592
 *            a(10) = 70608021 a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) -
 *            210*a(n-4) + 252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) +
 *            10*a(n-9) - a(n-10) for n>10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <limits.h>

/* coefficients for the linear recurrence (a(n) depends on the previous 10 terms) */
static const int coeffs[10] = {
    10, -45, 120, -210, 252,
   -210, 120, -45, 10, -1
};

/* small table of the first ten terms */
static const seq_t base_terms[10] = {
    (seq_t)200,
    (seq_t)4059,
    (seq_t)34350,
    (seq_t)181336,
    (seq_t)710976,
    (seq_t)2269938,
    (seq_t)6233356,
    (seq_t)15250675,
    (seq_t)34054592,
    (seq_t)70608021
};

/* cache for sequence values; cache[i] holds a(i+1) */
static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of valid terms currently stored */

/* Ensure that `cache` can hold at least `n` terms (n>=1).  New
   entries are zeroed but will be overwritten when needed. */
static void ensure_capacity(size_t n)
{
    if (n <= cache_size) return;
    /* grow by doubling strategy to reduce realloc frequency */
    size_t new_cap = cache_size ? cache_size : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                  /* memory allocation failed */
    for (size_t i = cache_size; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_size = new_cap;
}

/* Initialise cache with the first ten terms. */
static void init_base(void)
{
    ensure_capacity(10);
    for (size_t i = 0; i < 10; ++i) cache[i] = base_terms[i];
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined; return 0 for safety */

    init_base();                          /* guarantee first ten terms exist */

    if ((size_t)n <= 10) return cache[n-1];

    /* Extend cache up to n, computing missing values using the recurrence */
    ensure_capacity((size_t)n);
    for (size_t i = 10; i < (size_t)n; ++i)    /* i is zero‑based index of a(i+1) */
    {
        seq_t sum = 0;
        for (int k = 0; k < 10; ++k) {
            seq_t term = cache[i-1-k];          /* a(i - k) = a((i+1)-1-k) */
            int c = coeffs[k];
            if (c >= 0)
                sum += term * (seq_t)c;
            else
                sum -= term * (seq_t)(-c);
        }
        cache[i] = sum;                        /* store a(i+1) */
    }

    return cache[n-1];
}
