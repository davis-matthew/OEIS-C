/*
 * A015136  Consider Fibonacci-type sequences b(0)=X, b(1)=Y, b(k) = (b(k-1) +
 *          b(k-2)) mod n; all are periodic; sequence gives minimal nontrivial
 *          period.
 * https://oeis.org/A015136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive t such that det(M^t - I) shares a *
 *            factor with n, where * det = (-1)^t - L_t + 1, * L_t are Lucas
 *            numbers (L_0 = 2, L_1 = 1, L_(k+1) = L_k + L_(k-1)). * The
 *            determinant is computed modulo n, and t is the smallest *
 *            positive integer for which gcd(det, n) > 1. This t is exa...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* gcd for unsigned 64‑bit integers */
static inline uint64_t _gcd64(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

seq_t a(idx_t n) {
    /* offset 1: a(1)=1 */
    if (n == 1) return 1;

    uint64_t N = (uint64_t)n;          /* work in 64‑bit space */
    uint64_t L_prev = 2 % N;           /* L_0 */
    uint64_t L_cur   = 1 % N;           /* L_1 */

    for (idx_t t = 1;; ++t) {
        /* determinant of (M^t - I) mod N */
        uint64_t det;
        if (t & 1) {                   /* t odd : (-1)^t = -1 → det = -L_t */
            det = (L_cur == 0) ? 0 : N - L_cur;
        } else {                       /* t even: (-1)^t =  1 → det = 2 - L_t */
            if (2 >= L_cur) det = 2 - L_cur;
            else det = N - (L_cur - 2);
        }

        if (_gcd64(det, N) > 1) {
            return (seq_t)t;
        }

        /* advance to next Lucas number modulo N */
        uint64_t next = (L_prev + L_cur) % N;
        L_prev = L_cur;
        L_cur  = next;
    }
}
