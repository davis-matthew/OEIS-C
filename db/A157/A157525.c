/*
 * A157525  Triangle T(n, k) = n! * (Harmonic number(n-k) - Harmonic
 *          number(k)), read by rows.
 * https://oeis.org/A157525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row, k) where T(n,k)=n!*(H(n-k)-H(k)); H(m) = harmonic
 *            number(m) = sum_{i=1}^{m} 1/i. The sequence is read
 *            left‑to‑right in the rows defined by T
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 90, i64 n <= 230,
 *            i128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* floor(sqrt(x)) for an unsigned 128‑bit integer x */
static inline idx_t floor_sqrt_u128(__uint128_t x) {
    idx_t low = 0;
    idx_t high = (idx_t)~0ULL;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* dynamic storage for factorials */
static seq_t *fac = NULL;
static size_t fac_len = 0;
static size_t fac_cap = 0;

static void ensure_factorial(idx_t n) {
    if ((size_t)n >= fac_len) {
        size_t new_cap = fac_cap ? fac_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(fac, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        fac = tmp;
        fac_cap = new_cap;
        for (size_t i = fac_len; i <= (size_t)n; ++i) {
            if (i == 0) fac[0] = (seq_t)1;       /* seq_one() not guaranteed */
            else fac[i] = fac[i-1] * (seq_t)i;
        }
        fac_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n) {
    /* determine row r and column k (0‑based) for linear index n */
    __uint128_t val = (__uint128_t)8 * n + 1;
    idx_t sqrt_val = floor_sqrt_u128(val);
    idx_t r = (sqrt_val - 1) / 2;                     /* row index */
    idx_t offset = r * (r + 1) / 2;                   /* first index of row r */
    idx_t k = n - offset;                             /* column in the row */

    /* ensure factorial up to r is available */
    ensure_factorial(r);
    seq_t fac_r = fac[r];

    /* compute prefix sums of fac_r / i for i = 1..r  */
    seq_t pref_k = 0;
    seq_t pref_rk = 0;
    seq_t cur = 0;

    for (idx_t i = 1; i <= r; ++i) {
        seq_t term = fac_r / (seq_t)i;   /* integer division */
        cur += term;
        if (i == k)      pref_k = cur;
        if (i == r - k)  pref_rk = cur;
    }

    return pref_rk - pref_k;
}
