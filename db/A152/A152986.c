/*
 * A152986  Sum of proper divisors minus the number of proper divisors of
 *          pentagonal number A000326(n).
 * https://oeis.org/A152986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = sigma(A000326(n)) - divisor_count(A000326(n)) -
 *            A000326(n) + 1
 * Range:     u8 n <= 11, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000326(n) = n*(3*n-1)/2 is the n-th pentagonal number */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* compute pentagonal number P(n) = n*(3*n-1)/2 */
    idx_t three_n_minus_1 = (idx_t)3 * n - 1;
    seq_t P = (seq_t)n * (seq_t)three_n_minus_1 / 2;

    /* sum of all divisors of P and number of divisors of P */
    seq_t sigma = sigma_of((idx_t)P);              /* sum of all divisors */
    idx_t d = divisor_count_u64((uint64_t)P);     /* number of divisors */

    /* desired value: sum of proper divisors minus number of proper divisors */
    seq_t result = sigma - (seq_t)d - P + 1;
    return result;
}
