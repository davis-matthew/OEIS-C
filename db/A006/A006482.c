/*
 * A006482  Euler characteristics of polytopes.
 * https://oeis.org/A006482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1, a(3)=0, for n>=4: k=max(1,floor((n-1)/4)),
 *            a(n)=C(n-1,2k)-1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n) {
    if (n == 1 || n == 2) return 1;
    if (n == 3) return 0;
    
    idx_t k = (n - 1) / 4;
    if (k < 1) k = 1;
    
    seq_t result = binom(n - 1, 2 * k);
    return result - 1;
}
