/*
 * A145662  a(n) = numerator of polynomial of genus 1 and level n for m = 5 =
 *          A[1,n](5).
 * https://oeis.org/A145662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the rational sum S(n) = sum_{d=1}^{n-1}
 *            5^{n-d} / d The sequence a(n) is the numerator of S(n) in lowest
 *            terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 0;
    }
    seq_t num = 0;   /* current numerator of S(i) */
    seq_t den = 1;   /* current denominator of S(i) */

    for (idx_t i = 1; i < n; ++i) {   /* i represents the current n in S(n) */
        /* new fraction: S(i+1) = 5 * S(i) + 5 / (i) */
        seq_t term1 = num * (seq_t)i;   /* num * i */
        term1 *= (seq_t)5;              /* 5 * num * i */
        seq_t term2 = den * (seq_t)5;   /* 5 * den */
        seq_t new_num = term1 + term2;  /* (5*num*i + 5*den) * 1 */
        seq_t new_den = den * (seq_t)i; /* den * i */

        /* reduce to lowest terms */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
