/*
 * A214555  Subsequence of fixed points A099009 of the Kaprekar mapping, with
 *          numbers of the form 5{n}49{n+1}4{n}5, where d{n} means n-fold
 *          repetition of digit d.
 * https://oeis.org/A214555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=495 a(n) = ((100^(n+1)+10^(n+1)+1)*(10^(n+1)-2)+3)*5/9 for
 *            n>=1 Computes the fixed-point subsequence of the Kaprekar
 *            mapping
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n){
    if (n == 0)
        return (seq_t)495;
    idx_t k = n + 1;
    seq_t pow10_k  = pow10(k);         /* 10^(n+1) */
    seq_t pow10_2k = pow10(2 * k);     /* 10^(2n+2) == 100^(n+1) */
    seq_t term = (pow10_2k + pow10_k + (seq_t)1) * (pow10_k - (seq_t)2) + (seq_t)3;
    term = term * (seq_t)5 / (seq_t)9;
    return term;
}
