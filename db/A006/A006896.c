/*
 * A006896  a(n) is the number of hierarchical linear models on n labeled
 *          factors allowing 2-way interactions (but no higher order
 *          interactions); or the number of simple labeled graphs with nodes
 *          chosen from an n-set.
 * https://oeis.org/A006896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = sum_{k=0}^{n} C(n, k) * 2^(k*(k-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pw2.h"


seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom_nk = binom(n, k);
        idx_t exponent = k * (k - 1) / 2;
        seq_t power_of_2 = pw2(exponent);
        result += binom_nk * power_of_2;
    }
    
    return result;
}
