/*
 * A083708  G.f.: (x+4*x^3+x^5)/((1-x)^2*(1-x^2)^2*(1-x^3)^2).
 * https://oeis.org/A083708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the generating function
 *            (x+4*x^3+x^5)/((1-x)^2*(1-x^2)^2*(1-x^3)^2). The derived linear
 *            recurrence (order 12) is: a(n) = 2*a(n-1) + 1*a(n-2) - 2*a(n-3)
 *            - 3*a(n-4) + 0*a(n-5) + 6*a(n-6) + 0*a(n-7) - 3*a(n-8) -
 *            2*a(n-9) + 1*a(n-10) + 2*a(n-11) - 1*a(n-12), for n ≥ 12. For n
 *            < 12...
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 311, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All values are computed in the type seq_t. */

#include <stddef.h>

#define MAX_CACHE 20001   /* Must be ≥ maximum n expected (e.g., 10000). */

static seq_t cache[MAX_CACHE];
static idx_t last_computed = 0;
static int initialized = 0;

/* Coefficients d[1]..d[12] from the denominator expansion. */
static const int d[13] = {
    0,  /* d[0] unused */
    -2, -1,  2,  3,  0, -6,  0,  3,  2, -1, -2,  1
};

seq_t a(idx_t n)
{
    /* Lazy initialization of cache. */
    if (!initialized) {
        for (idx_t i = 0; i < MAX_CACHE; ++i)
            cache[i] = 0;
        cache[0] = 0;
        last_computed = 0;
        initialized = 1;
    }

    if (n >= MAX_CACHE) { /* Fallback: extend computation on the fly without storing. */
        /* Use a circular buffer of size 12. */
        static seq_t buffer[12] = {0};
        static idx_t buf_index = 0;
        static idx_t buf_filled = 0;

        /* If we have computed enough up to n, return cached value. */
        if (buf_filled > n) {
            return buffer[(buf_index + 12 - (buf_filled - n)) % 12];
        }

        /* Compute from last_filled up to n. */
        idx_t start = (buf_filled == 0) ? 0 : last_computed;
        for (idx_t i = start + 1; i <= n; ++i) {
            seq_t val = 0;

            /* Apply recurrence contributions. */
            for (int j = 1; j <= 12 && (idx_t)j <= i; ++j) {
                int coeff = d[j];
                if (coeff == 0)
                    continue;
                seq_t term = cache[i - (idx_t)j] * (seq_t)(coeff > 0 ? coeff : -coeff);
                if (coeff > 0)
                    val -= term;
                else
                    val += term;
            }

            /* Add numerator terms for n < 12. */
            if (i == 1)
                val += 1;
            else if (i == 3)
                val += 4;
            else if (i == 5)
                val += 1;

            cache[i] = val;
        }
        last_computed = n;
        return cache[n];
    }

    /* Compute all missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 0;

        /* Apply recurrence contributions. */
        for (int j = 1; j <= 12 && (idx_t)j <= i; ++j) {
            int coeff = d[j];
            if (coeff == 0)
                continue;
            seq_t term = cache[i - (idx_t)j] * (seq_t)(coeff > 0 ? coeff : -coeff);
            if (coeff > 0)
                val -= term;
            else
                val += term;
        }

        /* Add numerator terms for n < 12. */
        if (i == 1)
            val += 1;
        else if (i == 3)
            val += 4;
        else if (i == 5)
            val += 1;

        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
