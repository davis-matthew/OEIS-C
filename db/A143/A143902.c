/*
 * A143902  Rectangular array R by antidiagonals: R(m,n) = number of black
 *          squares.
 * https://oeis.org/A143902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143902 – Rectangular array R(m,n) = floor(m*n/2)
   enumerated by antidiagonals (sum m+n increasing, starting with (1,1)).
   For a given index n (n≥1) the coordinates (m,k) are:
     s  = smallest integer with (s-1)*s/2 ≥ n          // diagonal sum m+k
     offset = n - (s-2)*(s-1)/2 - 1                   // 0‑based position on this diagonal
     m       = 1 + offset
     k       = s - m
   Then a(n) = floor(m*k/2).
   The implementation uses only integer arithmetic on the supplied
   types seq_t and idx_t.  It runs in O(√n) time, well under the time
   limit for n up to about 10 000.  No auxiliary storage is required. */

#include <stdint.h>
#include <stddef.h>

/* If the build provides a generic integer square‑root function, it can be
   used.  Otherwise the function below is used. */
#ifdef _SERVER_
static inline idx_t _floor_isqrt_u64(idx_t n) {          /* use standard lib if available */
    return (idx_t)sqrtl((long double)n);
}
#else
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (idx_t bit = ((idx_t)1 << 62); bit != 0; bit >>= 2) {
        idx_t tmp = r | bit;
        if (tmp * tmp <= n) r = tmp;
    }
    return r;
}
#endif

/* Find the diagonal number s such that n lies on that diagonal.  */
static idx_t find_diagonal(idx_t n) {
    idx_t s = 2;                 /* first diagonal has sum 2 (m=1,n=1) */
    while ((s - 1) * s / 2 < n) ++s;
    return s;
}

seq_t a(idx_t n) {
    /* 1. locate the diagonal */
    idx_t s = find_diagonal(n);

    /* 2. terms before this diagonal */
    idx_t prev_terms = (s - 2) * (s - 1) / 2;
    /* 3. offset on current diagonal (0‑based) */
    idx_t offset = n - prev_terms - 1;
    /* 4. coordinates */
    idx_t m = 1 + offset;
    idx_t k = s - m;                 /* column index */

    /* 5. value floor(m*k/2) */
    seq_t prod = (seq_t)m * (seq_t)k;
    return prod / 2;
}
