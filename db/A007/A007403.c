/*
 * A007403  a(n) = Sum_{m=0..n} (Sum_{k=0..m} binomial(n,k))^3 =
 *          (n+2)*2^(3*n-1) - 3*2^(n-2)*n*binomial(2*n,n).
 * https://oeis.org/A007403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n+2)*2^(3*n-1) - 3*2^(n-2)*n*binomial(2*n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   binom, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"
#include "../utils/binom.h"


seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t temp = pw2(2 * n + 1) * (n_seq + 2) - 3 * n_seq * binom(2 * n, n);
    
    if (n == 0) {
        return temp >> 2;
    } else if (n == 1) {
        return temp >> 1;
    } else {
        return temp << (n - 2);
    }
}
