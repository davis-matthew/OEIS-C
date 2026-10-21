/*
 * A090103  n written in base equal to the number of distinct prime factors of
 *          n and a(1)=0.
 * https://oeis.org/A090103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n expressed in base ω(n), where ω(n) = number of distinct
 *            prime factors of n. a(1) = 0. For ω(n)=1 the base‑1
 *            representation is a string of n digits '1', i.e. (10ⁿ−1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   count_distinct_primes, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                   /* for size_t */
#include "../utils/count_distinct_primes.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1)              /* a(1) = 0 */
        return 0;

    /* number of distinct prime factors of n (ω(n)) */
    size_t k = (size_t)count_distinct_primes(n);

    /* Base‑1 special case */
    if (k <= 1) {
        /* (10ⁿ − 1)/9   -> 111…1  (n times)  */
        seq_t pow10n = pow_int((seq_t)10, n);
        return (pow10n - (seq_t)1) / (seq_t)9;
    }

    /* k ≥ 2 : ordinary base‑k conversion of n,
       then interpret the digits as a decimal number. */
    idx_t temp = n;
    unsigned char digits[64];   /* sufficient for n up to about 2³⁶ */
    size_t len = 0;

    while (temp > 0) {
        digits[len++] = temp % k;
        temp /= k;
    }

    seq_t result = 0;
    for (size_t i = len; i > 0; --i)          /* most significant digit first */
        result = result * 10 + (seq_t)digits[i - 1];

    return result;
}
