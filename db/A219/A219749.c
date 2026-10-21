/*
 * A219749  In the string b12b2b12 replace b with n 1's.
 * https://oeis.org/A219749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = concat(1^n,12,1^n,2,1^n,12)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t pow10_n = seq_pow(10, n);

    /* n repeated '1' digits: (10^n - 1) / 9 */
    seq_t ones = (pow10_n - 1) / 9;

    /* Construct the number via decimal concatenation */
    seq_t val = ones;                /* first block of 1^n */
    val = val * 100 + 12;            /* append '12'       */
    val = val * pow10_n + ones;      /* append second 1^n */
    val = val * 10 + 2;              /* append '2'        */
    val = val * pow10_n + ones;      /* append third 1^n  */
    val = val * 100 + 12;            /* append last '12'  */
    return val;
}
