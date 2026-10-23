/*
 * A132906  Numbers formed by concatenating 6 consecutive prime numbers.
 * https://oeis.org/A132906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(p_n, p_{n+1}, p_{n+2}, p_{n+3}, p_{n+4}, p_{n+5})
 *            where p_k is the k-th prime
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. No special handling for smaller n. */
    seq_t result = 0;
    idx_t i;
    for (i = 0; i < 6; ++i)
    {
        /* Retrieve the i-th consecutive prime starting from n. */
        seq_t prime = (seq_t)get_prime(n + i);

        /* Determine the number of decimal digits of the prime. */
        int digits = 0;
        seq_t tmp = prime;
        do
        {
            ++digits;
            tmp /= 10;
        } while (tmp > 0);

        /* Append the prime to the concatenated number. */
        result = result * pow10(digits) + prime;
    }
    return result;
}
