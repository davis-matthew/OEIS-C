/*
 * A003657  Discriminants of imaginary quadratic fields, negated.
 * https://oeis.org/A003657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Enumerate positive integers d where -d is a fundamental discriminant:
   d ≡ 3 (mod 4) and squarefree, or d ≡ 0 (mod 4) with d=4k, k ≡ 1,2 (mod 4), k squarefree. */

static bool is_squarefree(seq_t n) {
    if (n <= 1) return n == 1;
    for (seq_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < (seq_t)n) {
        num++;
        seq_t mod4 = num % 4;
        
        if (mod4 == 3) {
            if (is_squarefree(num)) count++;
        } else if (mod4 == 0) {
            seq_t k = num / 4;
            seq_t k_mod4 = k % 4;
            if ((k_mod4 == 1 || k_mod4 == 2) && is_squarefree(k)) count++;
        }
    }
    
    return num;
}
