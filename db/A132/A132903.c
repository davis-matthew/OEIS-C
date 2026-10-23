/*
 * A132903  Numbers formed by concatenating 3 consecutive prime numbers.
 * https://oeis.org/A132903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat( p_n, p_{n+1}, p_{n+2} )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k is the k-th prime (p_1 = 2) */

#include "../utils/get_prime.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Grab the three consecutive primes */
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);
    seq_t p3 = get_prime(n + 2);

    /* Concatenate them: (((p1 * 10^d2) + p2) * 10^d3) + p3 */
    seq_t res = p1;
    res = res * pow10(digit_len(p2)) + p2;
    res = res * pow10(digit_len(p3)) + p3;

    return res;
}
