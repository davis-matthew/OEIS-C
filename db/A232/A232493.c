/*
 * A232493  If n mod 2 = 0 then 2^n*3^(n-1)+2^(n+1)*3^(n/2-1) otherwise
 *          2^n*3^(n-1)+2^n*3^((n-1)/2).
 * https://oeis.org/A232493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   If n mod 2 = 0 then 2^n*3^(n-1)+2^(n+1)*3^(n/2-1) otherwise
 *            2^n*3^(n-1)+2^n*3^((n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t term1 = pow2(n) * pow3(n - 1);
    seq_t term2;
    if ((n & 1) == 0) {
        term2 = pow2(n + 1) * pow3((n / 2) - 1);
    } else {
        term2 = pow2(n) * pow3((n - 1) / 2);
    }
    return term1 + term2;
}
