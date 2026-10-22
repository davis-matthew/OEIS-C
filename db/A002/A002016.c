/*
 * A002016  Number of first n tetrahedral numbers (A000292) that are
 *          relatively prime to n.
 * https://oeis.org/A002016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of first n tetrahedral numbers (A000292) that are
 *            relatively prime to n
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

static seq_t tetrahedral(idx_t i) {
    seq_t a = (seq_t)i;
    seq_t b = a + 1;
    seq_t c = a + 2;
    
    if (a % 2 == 0) {
        a = a / 2;
    } else if (b % 2 == 0) {
        b = b / 2;
    } else {
        c = c / 2;
    }
    
    if (a % 3 == 0) {
        a = a / 3;
    } else if (b % 3 == 0) {
        b = b / 3;
    } else if (c % 3 == 0) {
        c = c / 3;
    }
    
    return a * b * c;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t b_i = tetrahedral(i);
        if (seq_gcd(b_i, n_seq) == 1) {
            count++;
        }
    }
    
    return count;
}
