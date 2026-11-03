/*
 * A006547  Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*i-1)!,i=1..n).
 * https://oeis.org/A006547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*i-1)!, i=1..n)
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    
    seq_t result = 0;
    seq_t factorial = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        if (i > 1) {
            factorial *= (seq_t)(2*i - 2);
            factorial *= (seq_t)(2*i - 1);
        }
        
        seq_t term = binomial(n, i) * pow2(i) * factorial;
        
        if (i % 2 == 1) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
