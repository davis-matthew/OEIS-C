/*
 * A006563  (2*n)!-Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*i-1)!,i=1..n).
 * https://oeis.org/A006563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n)! - Sum_{i=1}^{n} [(-1)^(i+1) * C(n,i) * 2^i *
 *            (2*i-1)!]
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t fact_2n = 1;
    for (idx_t j = 2; j <= 2 * n; j++) {
        fact_2n *= (seq_t)j;
    }
    
    seq_t result = fact_2n;
    seq_t odd_factorial = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        if (i > 1) {
            odd_factorial *= (seq_t)(2 * i - 2);
            odd_factorial *= (seq_t)(2 * i - 1);
        }
        
        seq_t term = binomial(n, i);
        term *= pow2(i);
        term *= odd_factorial;
        
        if (i % 2 == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
