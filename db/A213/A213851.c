/*
 * A213851  Least m such that m!*2^m >= n!
 * https://oeis.org/A213851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest m such that m!*2^m ≥ n! (offset 1)
 * Range:     u8 n <= 286, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static double *logfac = NULL;       /* logfac[i] = ln(i!) */
static size_t logfac_len = 0;       /* current allocated length */
static const double LOG2 = 0.693147180559945309417; /* ln(2) */

/* Extend logfac array to cover at least up to n (inclusive) */
static void extend_logfac(size_t n) {
    if (n + 1 <= logfac_len)
        return;
    size_t new_len = n + 1;
    double *new_arr = (double *)realloc(logfac, new_len * sizeof(double));
    if (!new_arr)
        abort();
    for (size_t i = logfac_len; i < new_len; ++i) {
        if (i == 0) {
            new_arr[i] = 0.0;
        } else {
            new_arr[i] = new_arr[i - 1] + log((double)i);
        }
    }
    logfac = new_arr;
    logfac_len = new_len;
}

/* Check whether m!*2^m ≥ n! using logarithms */
static bool ok(unsigned m, unsigned n) {
    return logfac[n] <= logfac[m] + m * LOG2;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    unsigned N = (unsigned)n;        /* n fits into unsigned in the intended range */

    extend_logfac(N);

    unsigned low = 1, high = N;
    while (low < high) {
        unsigned mid = low + (high - low) / 2;
        if (ok(mid, N))
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
