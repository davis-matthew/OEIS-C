/*
 * A055308  Number of labeled rooted trees with n nodes and 7 leaves.
 * https://oeis.org/A055308
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-7)(n-6)(n-5)(n-4)(n-3)(n-2)(n-1) * (63*n^5 -
 *            1890*n^4 + 22365*n^3 - 130186*n^2 + 371672*n - 414720) /
 *            14631321600 (offset 8)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean GCD for seq_t (works for built‑in and big‑integer types) */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Multiply `res` by `factor`, canceling common factors with `den` before
   and after the multiplication. */
static inline void mul_reduce(seq_t *res, seq_t factor, seq_t *den)
{
    if (factor == (seq_t)1)
        return;

    /* Cancel before multiplying. */
    seq_t g = gcd_seq(*res, *den);
    *res /= g;
    *den /= g;

    *res *= factor;

    /* Cancel again after the multiplication. */
    if (*den != (seq_t)1) {
        g = gcd_seq(*res, *den);
        *res /= g;
        *den /= g;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)8)
        return (seq_t)0;

    /* Denominator of the formula. */
    const seq_t DEN_CONST = (seq_t)14631321600u;
    seq_t den = DEN_CONST;

    /* Start with result 1. */
    seq_t res = (seq_t)1;

    /* Multiply n! part, cancelling with denominator. */
    for (idx_t i = (idx_t)2; i <= n; ++i)
        mul_reduce(&res, (seq_t)i, &den);

    /* Multiply extra product (n-7)...(n-1). */
    for (idx_t i = n - (idx_t)7; i <= n - (idx_t)1; ++i)
        mul_reduce(&res, (seq_t)i, &den);

    /* Compute polynomial part using 128‑bit integers. */
    __int128 sn   = (__int128)n;
    __int128 sn2  = sn * sn;
    __int128 sn3  = sn2 * sn;
    __int128 sn4  = sn3 * sn;
    __int128 sn5  = sn4 * sn;

    __int128 poly128 =
        ( __int128)63   * sn5
      - ( __int128)1890  * sn4
      + ( __int128)22365 * sn3
      - ( __int128)130186* sn2
      + ( __int128)371672* sn
      - ( __int128)414720;

    seq_t poly = (seq_t)poly128;

    /* Multiply polynomial part, canceling with remaining denominator. */
    mul_reduce(&res, poly, &den);

    /* At this point den should be 1; if not, the input n is too large for this
       data type and we simply return the truncated result. */
    return res;
}
