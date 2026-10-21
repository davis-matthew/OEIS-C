/*
 * A234225  Number of (n+1) X (6+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A234225 satisfies a 16‑term linear recurrence * * a(n) = 10
 *            a(n-1) +16 a(n-2) -460 a(n-3) +789 a(n-4) +7170 a(n-5) * -23944
 *            a(n-6) -39920 a(n-7) +246076 a(n-8) -38440 a(n-9) * -1115136
 *            a(n-10) +1151040 a(n-11) +2005056 a(n-12) * -3657600 a(n-13)
 *            -331776 a(n-14) +3456000 a(n-15) -1658880 a(n-16) *...
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[16] = {
    137536ULL,      293316ULL,      648536ULL,
    1597896ULL,     4055128ULL,     11145960ULL,
    31418264ULL,    94267656ULL,    289507096ULL,
    935746056ULL,   3091187096ULL,  10648070856ULL,
    37386104728ULL, 135693045000ULL, 499984167704ULL,
    1889644564296ULL
};

static const int rec_coeffs[16] = {
    10,   16,  -460,  789,   7170,  -23944,
   -39920, 246076, -38440, -1115136, 1151040,
   2005056, -3657600, -331776, 3456000, -1658880
};

static seq_t *cache    = NULL; /* stores a(k) at index k (a(0) unused) */
static size_t cap     = 0;     /* capacity in indices (highest index + 1) */
static size_t max_calculated = 0; /* highest n that has been computed */

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined; return 0. */
    if (n == 0) return 0;

    /* Ensure the cache is large enough for index n. */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        /* Existing entries already valid; new slots remain uninitialised. */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n, if not already done. */
    for (size_t k = max_calculated + 1; k <= (size_t)n; ++k) {
        if (k <= 16) {
            cache[k] = init_vals[k - 1];     /* initial values a(1)..a(16) */
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 0; j < 16; ++j) {
                seq_t term = cache[k - 1 - (size_t)j];  /* a(k-1-j) */
                int coeff   = rec_coeffs[j];
                if (coeff > 0)
                    pos += term * (seq_t)coeff;
                else
                    neg += term * (seq_t)(-coeff);
            }
            cache[k] = pos - neg;            /* pos ≥ neg always */
        }
    }
    max_calculated = (size_t)n;

    return cache[(size_t)n];
}
