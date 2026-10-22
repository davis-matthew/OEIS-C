/*
 * A006457  Number of elements in Z[ i ] whose 'smallest algorithm' is <= n.
 * https://oeis.org/A006457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2k) = 14*4^k - 34*2^k + 8*k + 21, a(2k+1) = 28*4^k - 48*2^k +
 *            8*k + 25
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   A001832_pow2, a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    seq_t pow2_k = A001832_pow2(k);
    seq_t pow4_k = a000248_pow((seq_t)4, k);
    
    if (n % 2 == 0) {
        seq_t pos = (seq_t)14 * pow4_k + (seq_t)8 * (seq_t)k + (seq_t)21;
        seq_t neg = (seq_t)34 * pow2_k;
        return pos - neg;
    } else {
        seq_t pos = (seq_t)28 * pow4_k + (seq_t)8 * (seq_t)k + (seq_t)25;
        seq_t neg = (seq_t)48 * pow2_k;
        return pos - neg;
    }
}
