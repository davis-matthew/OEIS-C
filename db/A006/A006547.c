/*
 * A006547  Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*i-1)!,i=1..n).
 * https://oeis.org/A006547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*i-1)!, i=1..n)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   binom, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pw2.h"

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
        
        seq_t term = binom(n, i) * pw2(i) * factorial;
        
        if (i % 2 == 1) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
