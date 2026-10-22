/*
 * A005698  Positions of remoteness 2 in Beans-Don't-Talk.
 * https://oeis.org/A005698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(3*k+3)+(-1)^k)/9 if n=2k-1, or (2^(3*k+5)-5*(-1)^k)/9
 *            if n=2k
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    idx_t k;
    seq_t val;
    
    if (n % 2 == 1) {
        k = (n + 1) / 2;
        val = pw2(3 * k + 3);
        if (k % 2 == 0) {
            return (val + 1) / 9;
        } else {
            return (val - 1) / 9;
        }
    } else {
        k = n / 2;
        val = pw2(3 * k + 5);
        if (k % 2 == 0) {
            return (val - 5) / 9;
        } else {
            return (val + 5) / 9;
        }
    }
}
