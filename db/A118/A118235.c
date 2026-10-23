/*
 * A118235  Smallest positive number starting an interval of consecutive
 *          integers with element sum n.
 * https://oeis.org/A118235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k such that k+(k+1)+...+(k+ℓ-1)=n for some ℓ ≥ 1
 * Range:     u8 n <= 88, u16 n <= 9907, u32 n <= 9907, u64 n <= 9907,
 *            u128 n <= 9907, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the smallest positive start of a consecutive integer interval whose
 * elements sum to n. The value fits in seq_t for all n that fit in seq_t. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* out of sequence bounds, not used */
    seq_t two_n = ((seq_t)n) * 2;          /* 2*n, sought divisor space */
    seq_t min_k = ((seq_t)n) + 1;          /* sentinel larger than any valid k */

    /* iterate over divisors d of 2*n, up to sqrt(2*n) */
    for (idx_t d = 1; ; ++d) {
        seq_t d_sq = (seq_t)d * (seq_t)d;
        if (d_sq > two_n) break;          /* passed the square root */

        if (two_n % (seq_t)d != 0) continue;   /* not a divisor */

        /* first divisor d */
        seq_t temp = two_n / (seq_t)d;          /* equals (2n)/d = e */
        seq_t x = temp - (seq_t)d + 1;          /* numerator for k */
        if (x % 2 == 0) {
            seq_t k = x / 2;
            if (k > 0 && k < min_k) min_k = k;
        }

        /* paired divisor e = temp, avoid double‑handling when d==e */
        if (temp != (seq_t)d) {
            seq_t e = temp;
            seq_t x2 = (seq_t)d - e + 1;        /* since 2n/e = d */
            if (x2 % 2 == 0) {
                seq_t k2 = x2 / 2;
                if (k2 > 0 && k2 < min_k) min_k = k2;
            }
        }
    }
    return min_k;
}
