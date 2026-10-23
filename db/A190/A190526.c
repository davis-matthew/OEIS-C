/*
 * A190526  Number of words of length n on alphabet {1,2,...,n} with no
 *          adjacent 1's.
 * https://oeis.org/A190526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..⌈n/2⌉} (n-1)^(n-i) * binomial(n-i+1, i)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute base^exp for arbitrary integer types */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Handle the trivial case a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t total = 0;
    /* ceil(n/2) for unsigned arithmetic */
    idx_t max_i = (n + 1) / 2;

    for (idx_t i = 0; i <= max_i; ++i) {
        /* binomial(n-i+1, i) */
        seq_t bin = binom(n - i + 1, i);
        /* (n-1)^(n-i) */
        seq_t power = pow_seq((seq_t)(n - 1), n - i);
        total += bin * power;
    }
    return total;
}
