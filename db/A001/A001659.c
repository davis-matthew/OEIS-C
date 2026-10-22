/*
 * A001659  Expansion of bracket function.
 * https://oeis.org/A001659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k)*binomial(n-1,k-1)*tau(k)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

static int a001659_fixed(void) {
#ifdef __cplusplus
    return !std::is_class<seq_t>::value;
#else
    return 1;
#endif
}

static idx_t *a001659_tau(idx_t n) {
    idx_t *t = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
    for (idx_t d = 1; d <= n; d++)
        for (idx_t m = d; m <= n; m += d)
            t[m]++;
    return t;
}

seq_t a(idx_t n) {
    idx_t *t = a001659_tau(n);
    seq_t res = 0;

    if (a001659_fixed()) {
        /* exact modulo 2^128 using additions only (Pascal row), then truncate */
        unsigned __int128 *row = (unsigned __int128 *)malloc((size_t)n * sizeof(unsigned __int128));
        row[0] = 1;
        for (idx_t r = 1; r < n; r++) {
            row[r] = 0;
            for (idx_t j = r; j >= 1; j--)
                row[j] += row[j - 1];
        }
        unsigned __int128 acc = 0;
        for (idx_t k = 1; k <= n; k++) {
            unsigned __int128 v = row[k - 1] * (unsigned __int128)t[k];
            if ((n - k) & 1) acc -= v;
            else acc += v;
        }
        free(row);
        uint64_t lo = (uint64_t)acc;
        uint64_t hi = (uint64_t)(acc >> 64);
        if (sizeof(seq_t) <= 8) {
            res = (seq_t)(int64_t)lo;
        } else {
            res = ((seq_t)(int64_t)hi << 64) | (seq_t)lo;
        }
    } else {
        seq_t b = 1; /* binomial(n-1, k-1) */
        for (idx_t k = 1; k <= n; k++) {
            seq_t term = b * (seq_t)t[k];
            if ((n - k) & 1) res -= term;
            else res += term;
            if (k < n) b = b * (seq_t)(n - k) / (seq_t)k;
        }
    }

    free(t);
    return res;
}
