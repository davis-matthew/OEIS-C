/*
 * A005153  Practical numbers: positive integers m such that every k <=
 *          sigma(m) is a sum of distinct divisors of m. Also called
 *          panarithmic numbers.
 * https://oeis.org/A005153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 7958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9948 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_practical(seq_t m) {
    if (m == 1) return true;
    if (m % 2 != 0) return false;
    
    seq_t remainder = m;
    seq_t sigma_prod = 1;
    
    seq_t power = 1;
    while (remainder % 2 == 0) {
        power *= 2;
        remainder /= 2;
    }
    sigma_prod = power * 2 - 1;
    
    for (seq_t p = 3; p * p <= remainder; p += 2) {
        if (remainder % p == 0) {
            if (p > sigma_prod + 1) return false;
            
            seq_t sigma_pk = 1;
            seq_t pj = 1;
            while (remainder % p == 0) {
                pj *= p;
                remainder /= p;
                sigma_pk += pj;
            }
            
            sigma_prod *= sigma_pk;
        }
    }
    
    if (remainder > 1) {
        if (remainder > sigma_prod + 1) return false;
    }
    
    return true;
}

/* Search: Practical numbers by Stewart's characterization theorem */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        if (is_practical(m)) {
            count++;
        }
    }
    
    return m;
}
