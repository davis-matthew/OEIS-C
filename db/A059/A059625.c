/*
 * A059625  Eleventh column (m=10) of convolution triangle A059594.
 * https://oeis.org/A059625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(k+16,16) * poly / 305235 where k = n/2 if n is
 *            even, else k = (n-1)/2. For even n: poly = 128k⁵ + 4768k⁴ +
 *            62272k³ + 336488k² + 673644k + 305235 For odd n: poly = 128k⁵ +
 *            6112k⁴ + 107968k³ + 860312k² + 2975580k + 3357585
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_i.h"

seq_t a(idx_t n) {
    const seq_t denom = (seq_t)305235;
    if ((n & 1) == 0) {                /* n even */
        idx_t k = n >> 1;
        seq_t binom = binomial_i(k + 16, 16);
        seq_t kk = (seq_t)k;

        /* Terms of the even polynomial */
        seq_t t1 = ((seq_t)128 * kk * kk * kk * kk * kk);
        seq_t t2 = ((seq_t)4768 * kk * kk * kk * kk);
        seq_t t3 = ((seq_t)62272 * kk * kk * kk);
        seq_t t4 = ((seq_t)336488 * kk * kk);
        seq_t t5 = ((seq_t)673644 * kk);
        seq_t t6 = (seq_t)305235;

        seq_t poly = t1 + t2 + t3 + t4 + t5 + t6;
        return (binom * poly) / denom;
    } else {                            /* n odd */
        idx_t k = n >> 1;               /* floor((n-1)/2) */
        seq_t binom = binomial_i(k + 16, 16);
        seq_t kk = (seq_t)k;

        /* Terms of the odd polynomial */
        seq_t t1 = ((seq_t)128 * kk * kk * kk * kk * kk);
        seq_t t2 = ((seq_t)6112 * kk * kk * kk * kk);
        seq_t t3 = ((seq_t)107968 * kk * kk * kk);
        seq_t t4 = ((seq_t)860312 * kk * kk);
        seq_t t5 = ((seq_t)2975580 * kk);
        seq_t t6 = (seq_t)3357585;

        seq_t poly = t1 + t2 + t3 + t4 + t5 + t6;
        return (binom * poly) / denom;
    }
}
