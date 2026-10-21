/*
 * A098572  a(n) = floor(Sum_{m=1..n} m^(1/m)).
 * https://oeis.org/A098572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( Σ_{m=1..n} m^(1/m) )
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* cache holds running sums up to each index */
static long double *sums = NULL;   /* sums[i] = Σ_{m=1..i+1} m^(1/m)  (i 0‑based) */
static idx_t    cap  = 0;          /* allocated size of sums */
static idx_t    last = 0;          /* 1‑based largest n whose sum is stored */
static long double cur_sum = 0.0L; /* cumulative sum up to last */

/* Return a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, this value is not used */

    /* allocate or enlarge cache if necessary */
    if (n > cap) {
        size_t new_cap = cap ? (cap << 1) : 64;
        while (new_cap < n) new_cap <<= 1;
        long double *tmp = (long double*)realloc(sums, new_cap * sizeof(long double));
        if (!tmp) return 0;          /* memory failure – return 0 as fallback */
        sums = tmp;
        cap = new_cap;
    }

    /* compute new terms if n is greater than previously computed */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            long double term = powl((long double)i, 1.0L/(long double)i);
            cur_sum += term;
            sums[i - 1] = cur_sum;  /* store at zero‑based index */
        }
        last = n;
    }

    /* floor the cumulative sum for n */
    long double val   = sums[n - 1];
    unsigned long long floor_val = (unsigned long long)floorl(val + 1e-12L);
    return (seq_t)floor_val;
}
