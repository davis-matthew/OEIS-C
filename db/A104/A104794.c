/*
 * A104794  Expansion of theta_4(q)^2 in powers of q.
 * https://oeis.org/A104794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = sum_{a,b∈ℤ, a²+b²=n} (-1)^{a+b}  */
/*  (computed by enumerating non‑negative pairs (a,b) and accounting for signs and multiplicities) */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit values. */
static inline idx_t int_sqrt(idx_t x)
{
    if (x == 0) return 0;
    idx_t low = 1, high = x / 2 + 1, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/*  Public function required by the problem statement. */
seq_t a(idx_t n)
{
    seq_t res = 0;                 /* accumulator for the coefficient */
    /* iterate a from 0 up to floor(sqrt(n)) without overflow */
    for (idx_t a = 0;; ++a) {
        unsigned __int128 a2 = (unsigned __int128)a * a;
        if (a2 > n) break;                     /* a² > n, stop the loop */

        idx_t rem = (idx_t)((unsigned __int128)n - a2); /* n - a² ≤ n fits in idx_t */
        idx_t b = int_sqrt(rem);
        if ((unsigned __int128)b * b != rem) continue; /* not a perfect square */

        /* sign = (-1)^{a+b} */
        seq_t sign = (((a + b) & 1) ? (seq_t)(-1) : (seq_t)1);

        if (a == 0 && b == 0) {
            res += sign;          /* multiplicity 1 */
        } else if (a == 0 || b == 0) {
            res += sign * (seq_t)2;  /* multiplicity 2 (± sign on the non‑zero coordinate) */
        } else {
            res += sign * (seq_t)4;  /* multiplicity 4 (± on each coordinate) */
        }
    }
    return res;
}
