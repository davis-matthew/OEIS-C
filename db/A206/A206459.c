/*
 * A206459  Number of 0..n arrays of length n+5 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{floor((n+5)/(n+1))} ((-1)^i
 *            (n+1)^((n+5)-(n+1)i) binomial((n+5)-n*i,i))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;                     // (n+1) as seq_t
    idx_t max_i = (n + 5) / (n + 1);             // floor((n+5)/(n+1))
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t exp = (n + 5) - (n + 1) * i;       // exponent for (n+1)^exp
        seq_t power = big_pow(n1, (seq_t)exp);   // (n+1)^exp
        seq_t bin = binomial((n + 5) - n * i, i); // C((n+5)-n*i, i)
        seq_t term = power * bin;
        if ((i & 1) == 0) {
            pos += term;
        } else {
            neg += term;
        }
    }

    return pos - neg;        // final result (positives dominate negatives)
}
