/*
 * A193383  G.f. is the imaginary part of the function C(x) that satisfies:
 *          C(x) = 1 + x/C(I*x).
 * https://oeis.org/A193383
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193383 – imaginary part of C(x) where C(x)=1+x/C(i*x).
   Formula: a(4k)==0,
            a(4k+1)=(-1)^(k+1)*C(2k,k)/2,
            a(4k+2)=(-1)^(k+1)*C(2k,k),
            a(4k+3)=(-1)^(k+1)*C(2k,k). */

#include <stdint.h>

/* compute binomial coefficient C(n,k)   (k ≤ n)   multiplicatively */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;         /* offset not defined for n<2 */

    unsigned r = (unsigned)(n % 4);
    if (r == 0) {                /* a(4k) == 0 */
        return 0;
    }

    idx_t k;
    seq_t val, sign;

    if (r == 1) {                /* a(4k+1) */
        k = (n - 1) / 4;
        val = binom(2 * k, k);
        val = val / 2;
        sign = ((k + 1) & 1) ? -1 : 1;
        return sign * val;
    } else {                     /* r==2 or r==3: a(4k+2) or a(4k+3) */
        k = (n - 2) / 4;
        val = binom(2 * k, k);
        sign = ((k + 1) & 1) ? -1 : 1;
        return sign * val;
    }
}
