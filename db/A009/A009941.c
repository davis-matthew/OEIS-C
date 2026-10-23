/*
 * A009941  Coordination sequence for Ni2In, Position Ni1 and In.
 * https://oeis.org/A009941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6)+c[n] * where c[0..6] =
 *            {1,10,35,40,35,10,1} and c[n]=0 for n>6
 * Range:     u8 n <= 4, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* The build system supplies seq_t and idx_t. */
extern seq_t a(idx_t n);

/* coefficients of the numerator of the generating function */
static const int coeff[7] = {1, 10, 35, 40, 35, 10, 1};

/* cache of previously computed values */
static seq_t *cache = NULL;
static idx_t max_cached = 0;

/* compute a(0)..a(n) inclusive */
static void compute_up_to(idx_t n)
{
    /* first allocation and base cases */
    if (max_cached == 0) {
        size_t sz = (size_t)(n > 6 ? n + 1 : 7);  /* at least indices 0..6 */
        cache = (seq_t *)realloc(cache, sz * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)47;
        cache[3] = (seq_t)98;
        cache[4] = (seq_t)179;
        cache[5] = (seq_t)275;
        /* cache[6] will be computed in the loop below if needed */
        max_cached = 5;
    }
    /* enlarge cache if necessary */
    if ((size_t)n + 1 > max_cached + 1) {
        size_t sz = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    }
    if (n <= max_cached)
        return;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t c_val = 0;
        if (i <= 6)
            c_val = (seq_t)coeff[i];

        /* recurrence using unsigned arithmetic */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-6] + c_val;
        sum += (~cache[i-4] + 1);   /* subtract cache[i-4] */
        sum += (~cache[i-5] + 1);   /* subtract cache[i-5] */
        cache[i] = sum;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
