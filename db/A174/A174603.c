/*
 * A174603  a(n) = n!*3^n/Gamma((n+1)/3) *sum_{k=0..floor(n/3)} 3^(-k)
 *          *Gamma((n+1)/3-k) / (k!*(n-3*k)!) , where Gamma (Greek capital
 *          gamma) is the Gamma-function.
 * https://oeis.org/A174603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*3^n/Gamma((n+1)/3) *sum_{k=0..floor(n/3)}
 *            3^(-k)*Gamma((n+1)/3-k)/(k!*(n-3*k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* base term: k = 0 -> 3^n */
    seq_t term = pow_seq((seq_t)3, (int)n);
    seq_t sum   = term;

    idx_t maxk = n / 3;            /* floor(n/3) */

    for (idx_t k = 0; k < maxk; ++k) {
        /* Compute term_{k+1} = term_k * (n-3k)*(n-3k-1) / (k+1) */
        seq_t num = term * (seq_t)(n - 3 * k);
        num *= (seq_t)(n - 3 * k - 1);
        seq_t denom = (seq_t)(k + 1);

        term = num / denom;
        sum += term;
    }

    return sum;
}
