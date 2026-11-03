/*
 * A108037  Triangle read by rows: n-th row is n-th nonzero Fibonacci number
 *          repeated n+1 times.
 * https://oeis.org/A108037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=0; for n>0 let r be the smallest integer with r(r+3)/2 ≥ n,
 *            then a(n) = F_r, the r‑th nonzero Fibonacci number
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 7380, big any n
 * Validated: standard, pass, 96 terms through n = 6886 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Return the r-th nonzero Fibonacci number (r>=1). */
static seq_t fib_nonzero(uint64_t r) {
    if (r <= 2) return (seq_t)1;
    seq_t a = 1, b = 1, tmp;
    for (uint64_t i = 3; i <= r; ++i) {
        tmp = b;
        b += a;
        a = tmp;
    }
    return b;
}

/* Compute the row index r for position n (n>=1). */
static uint64_t row_index(uint64_t n) {
    /* Quick return for the first row */
    if (n <= 2) return 1;

    uint64_t left = 1;
    /* Upper bound based on the inverse of r(r+3)/2 ≈ n → r ≈ sqrt(2n) */
    long double d = (long double)n;
    uint64_t right = (uint64_t)(2.0L * sqrtl(d) + 5.0L);
    if (right < left) right = 100;          /* safety net */

    while (left < right) {
        uint64_t mid = left + ((right - left) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 3) / 2;
        if (val >= (unsigned __int128)n)
            right = mid;
        else
            left = mid + 1;
    }
    return left;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    uint64_t r = row_index((uint64_t)n);
    return fib_nonzero(r);
}
