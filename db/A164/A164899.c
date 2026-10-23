/*
 * A164899  Binomial matrix (1,10^n) read by antidiagonals.
 * https://oeis.org/A164899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the triangle T(n,k) = A(n-k+1, k), where
 *            A(1,k) = 10^(k-1) and for n > 1, A(n,k)=A(n-1,k)+A(n-1,k-1). For
 *            a given n, let r be the row such that r*(r-1)/2 < n ≤ r*(r+1)/2,
 *            and let c = n - r*(r-1)/2. Then a(n) = sum_{i=0}^{min(c-1, r-c)}
 *            binom(r-c, i) * 10^(c-1-i)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 65, u64 n <= 230, u128 n <= 819,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Helper: find row index r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
static idx_t find_row(idx_t n) {
    idx_t low = 1;
    idx_t high = 1;
    /* Exponentially increase high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    low = high >> 1;
    /* Binary search [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* Out of range, defined as 0 */
    /* Find row r */
    idx_t r = find_row(n);

    /* Compute column c */
    unsigned __int128 prev_tri = (unsigned __int128)r * (r - 1) / 2;
    idx_t c = n - (idx_t)prev_tri;          /* 1‑based column */

    /* Parameters for the sum */
    idx_t m = r - c;                         /* n-k */
    idx_t t_max = (c - 1 < m) ? (c - 1) : m; /* floor(min(c-1, r-c)) */

    /* Prepare for iterative computation */
    seq_t result = 0;
    seq_t binom   = 1;                     /* binom(m, 0) */
    seq_t pow10_cur = pow10(c - 1);        /* 10^(c-1) */

    for (idx_t i = 0; i <= t_max; ++i) {
        seq_t term = binom * pow10_cur;
        result += term;

        if (i == t_max) break;           /* no further terms */

        /* Update for next i */
        pow10_cur = pow10_cur / 10;                 /* 10^(c-2-i) */
        binom = binom * (m - i) / (i + 1);          /* binom(m, i+1) */
    }
    return result;
}
