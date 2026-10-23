/*
 * A038324  Triangle read by rows whose (i,j)-th entry is
 *          binomial(i,j)*11^(i-j)*10^j.
 * https://oeis.org/A038324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 11^(i-j) * 10^j where i =
 *            floor((sqrt(8n+1)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 37, u64 n <= 138, u128 n <= 476,
 *            big any n
 * Helpers:   _my_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_my_binomial.h"

/* Forward declaration of util function if available */
#ifndef BINOMIAL_DECLARED
static seq_t _my_binomial(idx_t n, idx_t k);
#define BINOMIAL_DECLARED
#endif

seq_t a(idx_t n) {
    /* Determine row i and column j by incremental search
       (i.e., find smallest i with cumulative > n). */
    idx_t i = 0;
    idx_t cumulative = 1;          /* sum of sizes up to current row */
    while (n >= cumulative) {
        i++;
        cumulative += i + 1;
    }
    idx_t prev_cumulative = cumulative - (i + 1);
    idx_t j = n - prev_cumulative;

    /* Compute binomial(i, j) */
    seq_t bin = _my_binomial(i, j);

    /* Compute 11^(i-j) */
    seq_t pow11 = 1;
    for (idx_t k = 0; k < (i - j); ++k) {
        pow11 *= (seq_t)11;
    }

    /* Compute 10^j */
    seq_t pow10 = 1;
    for (idx_t k = 0; k < j; ++k) {
        pow10 *= (seq_t)10;
    }

    /* Combine */
    seq_t result = bin * pow11;
    result *= pow10;
    return result;
}
