/*
 * A078288  Least nontrivial multiple of the n-th prime beginning with 4.
 * https://oeis.org/A078288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least nontrivial multiple of the n-th prime that begins
 *            with 4 (search from 2 upwards until the product starts with
 *            digit 4)
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; values for n<1 are undefined */

    /* Prime p_n (fits in 32 bits for n up to 10000) */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;

    for (idx_t m = 2; ; ++m) {
        seq_t prod = prime * (seq_t)m;

        /* number of decimal digits of prod */
        int d = digit_len(prod);
        seq_t power = pow10((idx_t)(d - 1));      /* 10^(d-1) */

        seq_t start = power * (seq_t)4;           /* smallest number with d digits starting with 4 */
        seq_t end   = power * (seq_t)5;           /* smallest number with d digits starting with 5 */

        if (prod >= start && prod < end) {
            return prod;                         /* found the least multiple starting with 4 */
        }
    }
}
