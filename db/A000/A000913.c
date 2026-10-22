/*
 * A000913  Number of bond-rooted polyenoids with n edges.
 * https://oeis.org/A000913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*c(n+2) - (1/2)*c(n+1) - (3/4)*c((n+1)/2) +
 *            (1/2)*c((n-1)/4), c(n) = Catalan number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial_central
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial_central.h"

static seq_t catalan(idx_t k) {
    if (k == 0) return 1;
    seq_t binom = binomial_central(k);
    return binom / (k + 1);
}

seq_t a(idx_t n) {
    seq_t c_n2 = catalan(n + 2);
    seq_t c_n1 = catalan(n + 1);
    
    seq_t c_half = 0;
    if ((n + 1) % 2 == 0) {
        c_half = catalan((n + 1) / 2);
    }
    
    seq_t c_quarter = 0;
    if ((n - 1) % 4 == 0) {
        c_quarter = catalan((n - 1) / 4);
    }
    
    seq_t numerator = c_n2 - 2 * c_n1 - 3 * c_half + 2 * c_quarter;
    return numerator / 4;
}
