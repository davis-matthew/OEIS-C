/*
 * A120109  Row sums of number triangle A120108.
 * https://oeis.org/A120109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} lcm(1,...,n+1)/lcm(1,...,k+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Compute L_N = lcm(1,2,...,n+1) */
    seq_t L_N = (seq_t)1;
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t g = gcd(L_N, kseq);
        L_N = (L_N / g) * kseq;
    }

    /* Sum over k = 0..n of L_N / lcm(1,...,k+1) */
    seq_t sum = (seq_t)0;

    /* k = 0 term: lcm(1, ... ,1) = 1 */
    sum += L_N;     /* L_N / 1 */

    /* Compute lcm progressively for k >= 1 */
    seq_t L_k = (seq_t)1;  /* lcm(1, ... ,1) for k = 0 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kplus1 = (seq_t)(k + 1);
        seq_t g = gcd(L_k, kplus1);
        L_k = (L_k / g) * kplus1;   /* lcm(1,...,k+1) */
        seq_t ratio = L_N / L_k;    /* exact division since L_k divides L_N */
        sum += ratio;
    }

    return sum;
}
