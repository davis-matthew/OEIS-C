/*
 * A108794  Primes whose 10's complement is a semiprime, i.e., p is prime and
 *          10^L - p is a semiprime, where L is the number of digits in p.
 * https://oeis.org/A108794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_digits, is_prime, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose 10's complement is a semiprime
   a(n) is the nth prime p for which for L = number of digits of p,
   10^L − p is a semiprime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_semiprime_uint64.h"
#include "../utils/count_digits.h"

static const uint64_t pow10_arr[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe for safety */
    idx_t found = 0;
    uint64_t cand = 2;
    while (1) {
        if (is_prime(cand)) {
            idx_t L = count_digits(cand);
            uint64_t power = pow10_arr[L];
            uint64_t complement = power - cand;
            if (is_semiprime_uint64(complement)) {
                found++;
                if (found == n)
                    return (seq_t)cand;
            }
        }
        if (cand == 2)
            cand = 3;
        else
            cand += 2; /* skip even numbers */
    }
}
