/*
 * A006300  Number of rooted maps with n edges on torus.
 * https://oeis.org/A006300
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n-2} 2^(n-3-k)*(3^(n-1)-3^k)*binomial(n+k,k);
 *            the k=n-2 term equals 3^(n-2)*binomial(2n-2,n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t p3n1 = 1;                 /* 3^(n-1) */
    for (idx_t i = 0; i + 1 < n; i++) p3n1 *= 3;

    seq_t sum = 0;
    seq_t bin = 1;                  /* binomial(n+k,k) */
    seq_t p3k = 1;                  /* 3^k */
    seq_t p2 = 0;                   /* 2^(n-3-k) */
    if (n >= 3) {
        p2 = 1;
        for (idx_t i = 0; i + 3 < n; i++) p2 *= 2;
    }
    idx_t k = 0;
    for (; k + 2 < n; k++) {
        sum += p2 * (p3n1 - p3k) * bin;
        p2 /= 2;
        p3k *= 3;
        bin = bin * (seq_t)(n + k + 1) / (seq_t)(k + 1);
    }
    /* k = n-2: 2^(-1)*(3^(n-1)-3^(n-2)) = 3^(n-2) */
    sum += p3k * bin;
    return sum;
}
