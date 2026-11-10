/*
 * A076371  Occurrences of value=n in sequence A076370.
 * https://oeis.org/A076371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(n) - g(n-1), g(0) = 0, where g(m) = number of k >= 0
 *            with prime#(k) < prime#(m)^2 (equivalently, a(n) = number of k
 *            with A076370(k) = n); computed using cumulative logs of primes
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

static double *A076371_L = NULL;   /* L[k] = log(primorial(k)) */
static size_t A076371_cnt = 0;     /* number of valid entries */
static size_t A076371_limit = 0;

static void A076371_rebuild(size_t limit)
{
    char *s = (char *)malloc(limit + 1);
    if (!s) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= limit; ++i) s[i] = 1;
    s[0] = 0;
    if (limit >= 1) s[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (s[i])
            for (size_t j = i * i; j <= limit; j += i) s[j] = 0;
    size_t c = 0;
    for (size_t i = 2; i <= limit; ++i) if (s[i]) ++c;
    double *nl = (double *)realloc(A076371_L, (c + 1) * sizeof(double));
    if (!nl) exit(EXIT_FAILURE);
    A076371_L = nl;
    nl[0] = 0.0;
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (s[i]) { nl[k + 1] = nl[k] + log((double)i); ++k; }
    A076371_cnt = c + 1;
    A076371_limit = limit;
    free(s);
}

static size_t A076371_g(size_t m)
{
    if (m == 0) return 0;
    if (A076371_limit == 0) A076371_rebuild(1024);
    for (;;) {
        if (A076371_cnt > m) {
            double target = 2.0 * A076371_L[m];
            if (A076371_L[A076371_cnt - 1] > target) {
                size_t lo = 0, hi = A076371_cnt - 1;
                while (lo < hi) {
                    size_t mid = (lo + hi) >> 1;
                    if (A076371_L[mid] > target) hi = mid;
                    else lo = mid + 1;
                }
                return lo;
            }
        }
        A076371_rebuild(A076371_limit * 2);
    }
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    return (seq_t)(A076371_g(m) - A076371_g(m - 1));
}
