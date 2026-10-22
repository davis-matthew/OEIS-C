/*
 * A007674  Numbers m such that m and m+1 are squarefree.
 * https://oeis.org/A007674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Search for the n-th number m where both m and m+1 are squarefree */

static bool is_squarefree(seq_t n) {
    if (n <= 1) return n == 1;
    
    for (seq_t d = 2; d * d <= n; d++) {
        if (n % (d * d) == 0) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t m = 0;
    idx_t count = 0;
    
    while (count < n) {
        m++;
        if (is_squarefree(m) && is_squarefree(m + 1)) {
            count++;
        }
    }
    
    return m;
}
