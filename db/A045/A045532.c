/*
 * A045532  Concatenate n with n-th prime.
 * https://oeis.org/A045532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, A000040(n)) = n*10^(digits(p_n)) + p_n, where
 *            p_n is the n-th prime
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 9592, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Get n-th prime */
    uint32_t prime = get_prime(n);

    /* Number of decimal digits of that prime */
    uint64_t digit_count_pr = digit_count(prime);

    /* 10^digit_count */
    seq_t pow10_digits = pow10((idx_t)digit_count_pr);

    /* Concatenate n and prime: n * 10^digits + prime */
    seq_t result = ((seq_t)n * pow10_digits) + (seq_t)prime;

    return result;
}
