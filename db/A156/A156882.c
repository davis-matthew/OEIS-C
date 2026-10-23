/*
 * A156882  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1} ((k+1)^3
 *          - (k+1))^i ) with T(n, 0) = n!, read by antidiagonals.
 * https://oeis.org/A156882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) read by antidiagonals, where * T(n,k) = Π_{j=1..n}
 *            ( Σ_{i=0..j-1} ((k+1)^3-(k+1))^i ), * with T(n,0)=n! . * In the
 *            array, diagonal s = r+k. n starts at 0 and the * first element
 *            of diagonal s is at offset s*(s+1)/2. * The sequence is indexed
 *            as a(0)=1, a(1)=1, ... as in OEIS A156882
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 31, u64 n <= 41, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t int_sqrt(idx_t x) {
    /* Integer square root of x using binary search with 128‑bit arithmetic */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 33;                     /* > sqrt(8·2^64) */
    __int128 t;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        t = (__int128)mid * mid;
        if (t <= (__int128)x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    /* --- diagonal index, row r, column k ----------------------------- */
    __int128 disc128 = (__int128)1 + (__int128)8 * (unsigned long long)n;
    idx_t sqrt_disc = int_sqrt((idx_t)disc128);
    idx_t s = (sqrt_disc - 1) / 2;     /* diagonal number (r + k) */
    idx_t start = s * (s + 1) / 2;     /* first index of diagonal s */
    idx_t r = n - start;               /* r = row number on that diagonal */
    idx_t k = s - r;                   /* k = column number            */

    /* --- special case k == 0 (T(n,0) = n!) -------------------------- */
    if (k == 0) {
        seq_t fac = 1;
        for (idx_t i = 2; i <= r; ++i) fac *= i;
        return fac;
    }

    /* --- general case ------------------------------------------------ */
    /* X = k(k+1)(k+2).  All operations are performed in seq_t.         */
    seq_t X = (seq_t)k * (seq_t)(k + 1) * (seq_t)(k + 2);

    seq_t powX = 1;     /* X^0 initially */
    seq_t sum  = 1;     /* Σ_{i=0}^{j-1} X^i for current j */
    seq_t res  = 1;     /* running product */

    for (idx_t j = 1; j <= r; ++j) {
        res *= sum;          /* multiply by current sum_j */
        powX *= X;          /* prepare X^j for next sum */
        sum  += powX;       /* update Σ_{i=0}^{j} X^i for next round */
    }
    return res;
}
